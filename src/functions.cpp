#include "common.hpp"
#include "functions.hpp"
#include "git2.h"
#include "nlohmann/json.hpp"

using json = nlohmann::json;

void fetchAllAndPrune(git_repository *);
void assignDirPathBasedOnGitStatus(git_repository *, vector<string> &, vector<string> &, vector<string> &);
void listLocalBranches(git_repository *, vector<string> &);
void listRemoteBranches(git_repository *, vector<string> &, unordered_map<string, set<string>> &);

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

            fetchAllAndPrune(repo);

            assignDirPathBasedOnGitStatus(repo, repogitoriesWithoutChanges, repogitoriesUntracked, repogitoriesUncommited);

            vector<string> localBranches = {};
            vector<string> remotes = {};
            unordered_map<string, set<string>> remoteBranches;
            listLocalBranches(repo, localBranches);
            listRemoteBranches(repo, remotes, remoteBranches);
            cout << "Local branches:" << endl;
            for (auto localBranch : localBranches)
            {
                cout << " " << localBranch << endl;
            }
            cout << "Remote branches:" << endl;
            for (auto remote : remotes)
            {
                cout << " " << remote << endl;
                auto branches = remoteBranches[remote];
                for (auto remoteBranch : branches)
                {
                    cout << "  " << remoteBranch << endl;
                }
            }

            // TODO
            // diffBetweenLocalAndRemote()

            git_repository_free(repo);
        }

        fclose(inFile);

        json j;
        j["clean_repogitory"] = repogitoriesWithoutChanges;
        j["untracked_repogitory"] = repogitoriesUntracked;
        j["uncommited_repogitory"] = repogitoriesUncommited;

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

void assignDirPathBasedOnGitStatus(git_repository *repo, vector<string> &repogitoriesWithoutChanges, vector<string> &repogitoriesUntracked, vector<string> &repogitoriesUncommited)
{
    int error = 0;
    git_status_list *status = nullptr;
    const char *repo_path = git_repository_path(repo);
    string repo_path_str = repo_path;

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
    cout << repo_path_str << ", " << numOfDiff << endl;
    if (numOfDiff == 0)
    {
        repogitoriesWithoutChanges.push_back(repo_path_str);
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
            repogitoriesUntracked.push_back(repo_path_str);
        }
        if (hasUncommitedChange)
        {
            repogitoriesUncommited.push_back(repo_path_str);
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
