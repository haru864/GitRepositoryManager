#include <iostream>
#include <vector>
#include <fstream>
#include <unistd.h>
#include <string>
#include <limits.h>
#include <stdio.h>
#include <libgen.h>
#include <regex>

using namespace std;

const int BUFFER_SIZE = 256;
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

        string path = buffer;
        string trimmedPath = regex_replace(path, regex("/.git\n"), "\n");
        result.push_back(trimmedPath);
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
    if ((givenOption == "scan" || givenOption == "all") && argc < 3)
    {
        isAvailableOption = false;
    }
    if (!isAvailableOption)
    {
        cerr << "Given option is not available." << endl;
        cerr << "To see how to use the command, set --help as an argument." << endl;
        return 1;
    }

    char result[PATH_MAX];
    ssize_t count = readlink("/proc/self/exe", result, PATH_MAX);
    if (count == -1)
    {
        cerr << "Cannot read /proc/self/exe" << endl;
        return 1;
    }
    char *executableFileDirectory = dirname(result);
    char *parentDirectory = dirname(executableFileDirectory);
    string additionalPathForScanResult = "/tmp/repository_abspath_list.txt";
    string additionalPathForCheckResult = "/tmp/check_result.txt";
    const string scanResultFile = parentDirectory + additionalPathForScanResult;
    const string checkResultFile = parentDirectory + additionalPathForCheckResult;

    if (givenOption == "--help")
    {
        try
        {
            string additionalPathForManualFilePath = "/manual/manual.txt";
            string manualFile = parentDirectory + additionalPathForManualFilePath;
            FILE *f = fopen(manualFile.c_str(), "r");
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
            return 1;
        }
    }
    else if (givenOption == "scan")
    {
        try
        {
            const string dirPath = argv[2];
            vector<string> localRepoList = getLocalRepogitoryList(dirPath);
            FILE *f = fopen(scanResultFile.c_str(), "w");
            for (auto path : localRepoList)
            {
                fprintf(f, path.c_str());
            }
            fclose(f);
        }
        catch (const exception &e)
        {
            cerr << e.what() << endl;
            return 1;
        }
    }
    else if (givenOption == "check")
    {
    }
    else if (givenOption == "list")
    {
    }
    else if (givenOption == "all")
    {
    }

    return 0;
}
