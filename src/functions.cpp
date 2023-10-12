#include "common.hpp"
#include "functions.hpp"
#include "git2.h"

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

        FILE *f = fopen(scanResultFileAbsPath.c_str(), "r");
        char buffer[BUFFER_SIZE];

        while (fgets(buffer, BUFFER_SIZE, f) != NULL)
        {
            cerr << buffer << endl;

            // sample
            git_repository *repo;
            string path = "/home/haru/project/Recursion/Recursion_RemoteProcedureCall";
            int error = git_repository_open(&repo, path.c_str());
            if (error < 0)
            {
                const git_error *e = giterr_last();
                if (e)
                {
                    printf("Error %d: %s\n", e->klass, e->message);
                }
                else
                {
                    printf("Unknown error\n");
                }
                exit(EXIT_FAILURE);
            }
        }

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
