#include "common.hpp"
#include "functions.hpp"
#include "git2.h"
#include "nlohmann/json.hpp"

using json = nlohmann::json;

void fetchAllAndPrune(git_repository *);
void assignDirPathBasedOnGitStatus(string, git_repository *, vector<string> &, vector<string> &, vector<string> &);
void listLocalBranches(git_repository *, vector<string> &);
void listRemoteBranches(git_repository *, vector<string> &, unordered_map<string, set<string>> &);
bool hasLocalBranchesNotPushed(vector<string>, vector<string>, unordered_map<string, set<string>>);
bool hasDiffBetweenLocalAndRemote(git_repository *, vector<string>, unordered_map<string, set<string>>);
int diff_is_empty_cb(const git_diff_delta *, float, void *);

vector<string> getLocalRepogitoryList(string dirPath)
{
    const string command = "find " + dirPath + " -name \\.git";
    char buffer[BUFFER_SIZE];
    vector<string> result;

    FILE *pipe = popen(command.c_str(), "r");
    if (!pipe)
    {
        throw runtime_error("popen() failed!");
    }

    while (fgets(buffer, BUFFER_SIZE, pipe) != NULL)
    {

        string path = buffer;
        string trimmedPath = regex_replace(path, regex("/.git\n"), "\n");
        result.push_back(trimmedPath);
    }

    pclose(pipe);

    return result;
}

void help() noexcept
{
    try
    {
        FILE *f = fopen(manualFileAbsPath.c_str(), "r");
        char buffer[BUFFER_SIZE];
        while (fgets(buffer, sizeof(buffer), f) != NULL)
        {
            cout << buffer;
        }
        fclose(f);
    }
    catch (const exception &e)
    {
        cerr << e.what() << endl;
        exit(1);
    }
}

void scan(const string dirPath) noexcept
{
    try
    {
        vector<string> localRepoList = getLocalRepogitoryList(dirPath);
        ofstream file(scanResultFileAbsPath);
        for (auto path : localRepoList)
        {
            file << path;
        }
        file.close();
    }
    catch (const exception &e)
    {
        cerr << e.what() << endl;
        exit(1);
    }
}

void check() noexcept
{
    try
    {
        git_libgit2_init();

        FILE *inFile = fopen(scanResultFileAbsPath.c_str(), "r");
        char buffer[BUFFER_SIZE];
        vector<string> repogitoriesWithoutChanges = {};
        vector<string> repogitoriesUntracked = {};
        vector<string> repogitoriesUncommited = {};
        vector<string> repogitoriesNotPushed = {};
        vector<string> repogitoriesWithDiff = {};

        while (fgets(buffer, BUFFER_SIZE, inFile) != NULL)
        {
            git_repository *repo = nullptr;
            string pathWithLF = buffer;
            regex re("\n+$");
            string pathWithoutLF = regex_replace(pathWithLF, re, "");

            int error = git_repository_open(&repo, pathWithoutLF.c_str());
            if (error < 0)
            {
                const git_error *e = git_error_last();
                printf("Error %d/%d: %s\n", error, e->klass, e->message);
                exit(error);
            }

            // git fetch -all, git fetch --prune
            fetchAllAndPrune(repo);

            // git status
            assignDirPathBasedOnGitStatus(pathWithoutLF, repo, repogitoriesWithoutChanges, repogitoriesUntracked, repogitoriesUncommited);

            vector<string> localBranches = {};
            vector<string> remotes = {};
            unordered_map<string, set<string>> remoteBranches;
            listLocalBranches(repo, localBranches);
            listRemoteBranches(repo, remotes, remoteBranches);
            cout << "Local branches:" << endl;
            for (auto localBranch : localBranches)
            {
                cout << "  " << localBranch << endl;
            }
            cout << "Remote branches:" << endl;
            for (auto remote : remotes)
            {
                cout << "  " << remote << endl;
                auto branches = remoteBranches[remote];
                for (auto remoteBranch : branches)
                {
                    cout << "    " << remoteBranch << endl;
                }
            }

            // どのリモートにも存在しないローカルブランチをリストアップする
            if (hasLocalBranchesNotPushed(localBranches, remotes, remoteBranches))
            {
                repogitoriesNotPushed.push_back(pathWithoutLF);
            }

            // TODO
            // リモートごとにローカルブランチとgit diffして、差分がないかチェックする
            if (hasDiffBetweenLocalAndRemote(repo, localBranches, remoteBranches))
            {
                repogitoriesWithDiff.push_back(pathWithoutLF);
            }

            git_repository_free(repo);
        }

        fclose(inFile);

        json j;
        j["clean"] = repogitoriesWithoutChanges;
        j["untracked_changes"] = repogitoriesUntracked;
        j["uncommited_changes"] = repogitoriesUncommited;
        j["unpushed_branch"] = repogitoriesNotPushed;
        j["difference_branch"] = repogitoriesWithDiff;

        ofstream file(checkResultFileAbsPath);
        file << j << endl;
        file.close();

        git_libgit2_shutdown();
    }
    catch (const exception &e)
    {
        cerr << e.what() << endl;
        exit(1);
    }
}

void list() noexcept
{
    try
    {
        repogitories.clear();
        ifstream f(checkResultFileAbsPath);
        json jsonData = json::parse(f);
        f.close();
        for (json::iterator it = jsonData.begin(); it != jsonData.end(); ++it)
        {
            cout << "---" << it.key() << "---" << endl;
            for (auto elem : it.value())
            {
                repogitories.push_back(elem);
                cout << repogitories.size() << "." << elem << endl;
            }
            cout << endl;
        }
    }
    catch (const exception &e)
    {
        cerr << e.what() << endl;
        exit(1);
    }
}

vector<string> splitString(string str, string separator)
{
    vector<string> result;
    auto offset = string::size_type(0);
    while (1)
    {
        auto pos = str.find(separator, offset);
        if (pos == string::npos)
        {
            result.push_back(str.substr(offset));
            break;
        }
        result.push_back(str.substr(offset, pos - offset));
        offset = pos + separator.length();
    }
    return result;
}

bool isRepogitoryNumber(string numberStr)
{
    try
    {
        int numberInt = stoi(numberStr);
        int repogitoryIndex = numberInt - 1;
        string repogitoryPath = repogitories[repogitoryIndex];
        return true;
    }
    catch (const invalid_argument &e)
    {
        return false;
    }
    catch (const out_of_range &e)
    {
        return false;
    }
    catch (...)
    {
        return false;
    }
    return false;
}

void assignDirPathBasedOnGitStatus(string dirPath, git_repository *repo, vector<string> &repogitoriesWithoutChanges, vector<string> &repogitoriesUntracked, vector<string> &repogitoriesUncommited)
{
    int error = 0;
    git_status_list *status = nullptr;

    git_status_options statusopt = GIT_STATUS_OPTIONS_INIT;
    statusopt.show = GIT_STATUS_SHOW_INDEX_AND_WORKDIR;
    statusopt.flags = GIT_STATUS_OPT_INCLUDE_UNTRACKED |
                      GIT_STATUS_OPT_RENAMES_HEAD_TO_INDEX |
                      GIT_STATUS_OPT_SORT_CASE_SENSITIVELY;

    error = git_status_list_new(&status, repo, &statusopt);
    if (error != 0)
    {
        git_repository_free(repo);
        const git_error *e = git_error_last();
        printf("Error %d/%d: %s\n", error, e->klass, e->message);
        exit(error);
    }

    size_t numOfDiff = git_status_list_entrycount(status);
    cout << dirPath << ", changes: " << numOfDiff << endl;
    if (numOfDiff == 0)
    {
        repogitoriesWithoutChanges.push_back(dirPath);
    }
    else
    {
        const git_status_entry *s;
        bool hasUntrackedChange = false;
        bool hasUncommitedChange = false;
        for (size_t i = 0; i < numOfDiff; ++i)
        {
            s = git_status_byindex(status, i);
            if (s->head_to_index)
            {
                cout << "Modified: " << s->head_to_index->old_file.path << endl;
                hasUncommitedChange = true;
            }
            else if (s->index_to_workdir)
            {
                cout << "Untracked: " << s->index_to_workdir->old_file.path << endl;
                hasUntrackedChange = true;
            }
        }
        if (hasUntrackedChange)
        {
            repogitoriesUntracked.push_back(dirPath);
        }
        if (hasUncommitedChange)
        {
            repogitoriesUncommited.push_back(dirPath);
        }
    }

    git_status_list_free(status);
}

void fetchAllAndPrune(git_repository *repo)
{
    git_strarray remotes = {0};
    if (git_remote_list(&remotes, repo) != 0)
    {
        const git_error *e = git_error_last();
        cerr << "Error getting remotes: " << e->message << endl;
        exit(1);
    }

    for (size_t i = 0; i < remotes.count; ++i)
    {
        git_remote *remote = nullptr;
        const char *remote_name = remotes.strings[i];

        if (git_remote_lookup(&remote, repo, remote_name) != 0)
        {
            const git_error *e = git_error_last();
            cerr << "Error looking up remote '" << remote_name << "': " << e->message << endl;
            continue;
        }

        if (git_remote_fetch(remote, nullptr, nullptr, nullptr) != 0)
        {
            const git_error *e = git_error_last();
            cerr << "Error fetching remote '" << remote_name << "': " << e->message << endl;
        }

        git_fetch_options fetch_opts = GIT_FETCH_OPTIONS_INIT;
        fetch_opts.prune = GIT_FETCH_PRUNE;

        if (git_remote_fetch(remote, nullptr, &fetch_opts, nullptr) != 0)
        {
            const git_error *e = git_error_last();
            cerr << "Error pruning remote '" << remote_name << "': " << e->message << endl;
        }

        git_remote_free(remote);
    }

    git_strarray_dispose(&remotes);
}

void listLocalBranches(git_repository *repo, vector<string> &localBranches)
{
    git_branch_iterator *it;
    if (git_branch_iterator_new(&it, repo, GIT_BRANCH_LOCAL) != 0)
    {
        const git_error *e = git_error_last();
        cerr << "Error creating branch iterator: " << e->message << endl;
        exit(1);
    }

    git_reference *ref;
    git_branch_t type;
    while (git_branch_next(&ref, &type, it) == 0)
    {
        const char *localBranchName;
        if (git_branch_name(&localBranchName, ref) != 0)
        {
            const git_error *e = git_error_last();
            cerr << "Error retrieving branch name: " << e->message << endl;
            continue;
        }
        localBranches.push_back(localBranchName);
        git_reference_free(ref);
    }

    git_branch_iterator_free(it);
}

void listRemoteBranches(git_repository *repo, vector<string> &remotes, unordered_map<string, set<string>> &remoteBranches)
{
    git_branch_iterator *it;
    if (git_branch_iterator_new(&it, repo, GIT_BRANCH_REMOTE) != 0)
    {
        const git_error *e = git_error_last();
        cerr << "Error creating branch iterator: " << e->message << endl;
        exit(1);
    }

    git_reference *ref;
    git_branch_t type;
    while (git_branch_next(&ref, &type, it) == 0)
    {
        const char *remoteBranchName;
        if (git_branch_name(&remoteBranchName, ref) != 0)
        {
            const git_error *e = git_error_last();
            cerr << "Error retrieving branch name: " << e->message << endl;
            continue;
        }
        string remoteBranchNameStr(remoteBranchName);
        size_t pos = remoteBranchNameStr.find('/');
        string remoteName = remoteBranchNameStr.substr(0, pos);
        string branchName = remoteBranchNameStr.substr(pos + 1);
        remotes.push_back(remoteName);
        remoteBranches[remoteName].insert(branchName);

        git_reference_free(ref);
    }

    git_branch_iterator_free(it);
}

bool hasLocalBranchesNotPushed(vector<string> localBranches, vector<string> remotes, unordered_map<string, set<string>> remoteBranches)
{
    set<string> remoteBranchSet = {};
    for (const auto remote_to_branches : remoteBranches)
    {
        auto remote = remote_to_branches.first;
        auto branches = remote_to_branches.second;
        for (const auto branch : branches)
        {
            remoteBranchSet.insert(branch);
        }
    }
    for (const auto localBranch : localBranches)
    {
        if (remoteBranchSet.find(localBranch) == remoteBranchSet.end())
        {
            return true;
        }
    }
    return false;
}

bool hasDiffBetweenLocalAndRemote(git_repository *repo, vector<string> localBranches, unordered_map<string, set<string>> remoteBranches)
{
    int error = 0;
    git_commit *local_commit = NULL;
    git_commit *remote_commit = NULL;
    git_tree *local_tree = NULL;
    git_tree *remote_tree = NULL;
    git_diff *diff_local_to_remote = NULL;
    git_diff *diff_remote_to_local = NULL;
    const git_error *e = NULL;
    bool hasDiff = false;

    for (const auto remote_to_branches : remoteBranches)
    {
        const string remote_name = remote_to_branches.first;
        const set<string> remote_branch_name_set = remote_to_branches.second;

        for (const string local_branch_name : localBranches)
        {
            const auto itr = remote_branch_name_set.find(local_branch_name);
            if (itr == remote_branch_name_set.end())
            {
                continue;
            }

            string remote_branch_name = remote_name + "/" + *itr;
            cout << "checking difference between " << local_branch_name << " and " << remote_branch_name << endl;

            // ローカルブランチの最新コミットを取得
            error = git_revparse_single((git_object **)&local_commit, repo, local_branch_name.c_str());
            if (error != 0)
            {
                e = git_error_last();
                std::cerr << "Error finding local branch commit: " << e->message << std::endl;
                goto cleanup;
            }

            // リモートブランチの最新コミットを取得
            error = git_revparse_single((git_object **)&remote_commit, repo, remote_branch_name.c_str());
            if (error != 0)
            {
                e = git_error_last();
                std::cerr << "Error finding remote branch commit: " << e->message << std::endl;
                goto cleanup;
            }

            // 各コミットからツリーを取得
            error = git_commit_tree(&local_tree, local_commit);
            if (error != 0)
            {
                e = git_error_last();
                std::cerr << "Error getting local commit tree: " << e->message << std::endl;
                goto cleanup;
            }

            error = git_commit_tree(&remote_tree, remote_commit);
            if (error != 0)
            {
                e = git_error_last();
                std::cerr << "Error getting remote commit tree: " << e->message << std::endl;
                goto cleanup;
            }

            // 二つのツリー間の差分を計算
            error = git_diff_tree_to_tree(&diff_local_to_remote, repo, local_tree, remote_tree, NULL);
            if (error != 0)
            {
                e = git_error_last();
                std::cerr << "Error creating diff: " << e->message << std::endl;
                goto cleanup;
            }

            error = git_diff_tree_to_tree(&diff_remote_to_local, repo, remote_tree, local_tree, NULL);
            if (error != 0)
            {
                e = git_error_last();
                std::cerr << "Error creating diff: " << e->message << std::endl;
                goto cleanup;
            }

            bool diff_is_empty = true;
            git_diff_foreach(diff_local_to_remote, diff_is_empty_cb, NULL, NULL, NULL, &diff_is_empty);
            if (diff_is_empty == false)
            {
                return true;
            }
            git_diff_foreach(diff_remote_to_local, diff_is_empty_cb, NULL, NULL, NULL, &diff_is_empty);
            if (diff_is_empty == false)
            {
                return true;
            }
        }
    }

    return false;

cleanup:
    git_commit_free(local_commit);
    git_commit_free(remote_commit);
    git_tree_free(local_tree);
    git_tree_free(remote_tree);
    git_diff_free(diff_local_to_remote);
    git_diff_free(diff_remote_to_local);

    throw runtime_error("Error occured when take a difference between local and remote");
}

int diff_is_empty_cb(const git_diff_delta *delta, float progress, void *payload)
{
    *(bool *)payload = false;
    return 1;
}
