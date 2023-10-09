#include "common.hpp"
#include "functions.hpp"

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
        FILE *f = fopen(scanResultFileAbsPath.c_str(), "r");
        char buffer[BUFFER_SIZE];
        while (fgets(buffer, BUFFER_SIZE, f) != NULL)
        {
        }
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
