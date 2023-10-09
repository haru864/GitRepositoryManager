#include "common.hpp"

int BUFFER_SIZE;
vector<string> AVAILABLE_OPTION;
string scanResultFileAbsPath;
string checkResultFileAbsPath;
string manualFileAbsPath;

void initGlobalVariables()
{
    BUFFER_SIZE = 256;
    AVAILABLE_OPTION = {"scan", "check", "list", "all", "--help"};
    char result[PATH_MAX];
    ssize_t count = readlink("/proc/self/exe", result, PATH_MAX);
    if (count == -1)
    {
        cerr << "Cannot read /proc/self/exe" << endl;
        exit(1);
    }
    char *executableFileDirectory = dirname(result);
    char *parentDirectory = dirname(executableFileDirectory);
    string additionalPathForScanResult = "/tmp/repository_abspath_list.txt";
    string additionalPathForCheckResult = "/tmp/check_result.txt";
    string additionalPathForManualFilePath = "/manual/manual.txt";
    scanResultFileAbsPath = parentDirectory + additionalPathForScanResult;
    checkResultFileAbsPath = parentDirectory + additionalPathForCheckResult;
    manualFileAbsPath = parentDirectory + additionalPathForManualFilePath;
}
