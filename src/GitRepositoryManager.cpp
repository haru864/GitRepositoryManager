#include <iostream>
#include <vector>
#include <fstream>
#include <unistd.h>
#include <string>
#include <limits.h>
#include <stdio.h>
#include <libgen.h>

using namespace std;

const int BUFFER_SIZE = 256;
const string HELP_OPTION = "--help";
const vector<string> AVAILABLE_OPTION = {"scan", "check", "list", "all", "--help"};

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

        string line = buffer;

        result.push_back(line);
    }

    pclose(pipe);

    return result;
}

int main(int argc, char **argv)
{
    cout << "-----------------debug-----------------" << endl;
    cout << "argc " << argc << endl;
    for (int i = 0; i < argc; i++)
    {
        cout << "argv[" << i << "]" << argv[i] << endl;
    }
    cout << "---------------------------------------" << endl;

    if (argc != 2 && argc != 3)
    {
        cerr << "Too many or too few arguments." << endl;
        cerr << "To see how to use the command, set --help as an argument." << endl;
        return 1;
    }

    string givenOption = argv[1];
    bool isAvailableOption = false;
    for (auto option : AVAILABLE_OPTION)
    {
        if (givenOption == option)
        {
            isAvailableOption = true;
        }
    }
    if (!isAvailableOption)
    {
        cerr << "GIven option is not available." << endl;
        cerr << "To see how to use the command, set --help as an argument." << endl;
        return 1;
    }

    if (argv[1] == HELP_OPTION)
    {
        try
        {
            char result[PATH_MAX];
            ssize_t count = readlink("/proc/self/exe", result, PATH_MAX);
            if (count == -1)
            {
                throw runtime_error("Cannot read manual file.");
            }
            char *executableFileDirectory = dirname(result);
            char *parentDirectory = dirname(executableFileDirectory);
            string additionalPathForManualFilePath = "/manual/manual.txt";
            string manualFileDirectory = parentDirectory + additionalPathForManualFilePath;
            FILE *f = fopen(manualFileDirectory.c_str(), "r");
            char buffer[BUFFER_SIZE];
            while (fgets(buffer, sizeof(buffer), f) != NULL)
            {
                cout << buffer;
            }
        }
        catch (const exception &e)
        {
            cerr << e.what() << endl;
            return 1;
        }
        return 0;
    }

    string dirPath = "";

    // vector<string> localRepoList = getLocalRepogitoryList(dirPath);

    return 0;
}
