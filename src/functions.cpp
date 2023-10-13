#include "common.hpp"
#include "functions.hpp"
#include "git2.h"
#include "nlohmann/json.hpp"

using json = nlohmann::json;

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
        FILE *f = fopen(scanResultFileAbsPath.c_str(), "w");
        for (auto path : localRepoList)
        {
            fprintf(f, path.c_str());
        }
        fclose(f);
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
        vector<string> repoListWithUnpushedChange = {};

        while (fgets(buffer, BUFFER_SIZE, inFile) != NULL)
        {
            git_repository *repo = nullptr;
            git_status_list *status = nullptr;

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

            git_status_options statusopt = GIT_STATUS_OPTIONS_INIT;
            statusopt.show = GIT_STATUS_SHOW_INDEX_AND_WORKDIR;
            statusopt.flags = GIT_STATUS_OPT_INCLUDE_UNTRACKED |
                              GIT_STATUS_OPT_RENAMES_HEAD_TO_INDEX |
                              GIT_STATUS_OPT_SORT_CASE_SENSITIVELY;

            // ステータスの取得
            error = git_status_list_new(&status, repo, &statusopt);
            if (error != 0)
            {
                git_repository_free(repo);
                const git_error *e = git_error_last();
                printf("Error %d/%d: %s\n", error, e->klass, e->message);
                exit(error);
            }

            // ステータスの数を取得
            size_t maxi = git_status_list_entrycount(status);
            cout << pathWithoutLF << ", " << maxi << endl;
            if (maxi > 0)
            {
                repoListWithUnpushedChange.push_back(pathWithoutLF);
            }
            const git_status_entry *s;
            for (size_t i = 0; i < maxi; ++i)
            {
                s = git_status_byindex(status, i);
                if (s->status == GIT_STATUS_CURRENT)
                {
                    continue;
                }

                // ここでステータス情報(s)を使って何か処理を行う
                // 例えば、変更されたファイルのパスを出力する
                if (s->head_to_index)
                {
                    cout << "Modified: " << s->head_to_index->old_file.path << endl;
                }
                else if (s->index_to_workdir)
                {
                    cout << "Untracked: " << s->index_to_workdir->old_file.path << endl;
                }
            }

            git_status_list_free(status);
            git_repository_free(repo);
        }

        json j;
        // json j["repogitory_with_unpushed_changes"] = repoListWithUnpushedChange;
        j["list"] = {1, 0, 2};
        // FILE *outFile = fopen(checkResultFileAbsPath.c_str(), "w");
        // fprintf(outFile, j.get<string>().c_str());
        fclose(inFile);
        // fclose(outFile);

        ofstream file(checkResultFileAbsPath);
        file << j << endl;

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
}

void all(const string dirPath) noexcept
{
}
