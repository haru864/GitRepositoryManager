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
        vector<string> repogitoriesNotDifferFromRemote = {};
        vector<string> repogitoriesDifferFromRemote = {};

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

            error = git_status_list_new(&status, repo, &statusopt);
            if (error != 0)
            {
                git_repository_free(repo);
                const git_error *e = git_error_last();
                printf("Error %d/%d: %s\n", error, e->klass, e->message);
                exit(error);
            }

            size_t numOfDiff = git_status_list_entrycount(status);
            cout << pathWithoutLF << ", " << numOfDiff << endl;
            if (numOfDiff > 0)
            {
                repogitoriesNotDifferFromRemote.push_back(pathWithoutLF);
            }
            else
            {
                repogitoriesDifferFromRemote.push_back(pathWithoutLF);
            }

            git_status_list_free(status);
            git_repository_free(repo);
        }

        fclose(inFile);

        json j;
        j["repogitory_not_doffer_from_remote"] = repogitoriesNotDifferFromRemote;
        j["repogitory_doffer_from_remote"] = repogitoriesDifferFromRemote;

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
        ifstream f(checkResultFileAbsPath);
        json jsonData = json::parse(f);
        f.close();
        // cout << jsonData.dump() << endl;
        for (json::iterator it = jsonData.begin(); it != jsonData.end(); ++it)
        {
            cout << "<" << it.key() << ">" << endl;
            for (auto elem : it.value())
            {
                cout << elem << endl;
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
