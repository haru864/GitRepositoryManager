#include <iostream>
#include <fstream>
#include <unistd.h>
#include <signal.h>
#include <string>
#include <limits.h>

int main(int argc, char **argv)
{
    char result[PATH_MAX];
    ssize_t count = readlink("/proc/self/exe", result, PATH_MAX);
    if (count == -1)
    {
        std::cerr << "Failed to get file path" << std::endl;
        return 1;
    }
    std::string absPath(result, count);
    std::cout << path << std::endl;

    if (daemon(0, 0) == -1)
    {
        std::cerr << "Failed to start 'GitRepositoryManager' as daemon" << std::endl;
        return 1;
    }

    std::ofstream logFile("/home/haru/project/GitRepositoryManager/test/output/sample_output.txt");
    logFile << "test" << std::endl;

    // while (running)
    // {
    //     std::string input;
    //     std::getline(std::cin, input);
    //     if (input == "hogehoge")
    //     {
    //         logFile << "[GitRepositoryManager] Detected that a local Git repository has been created." << std::endl;
    //     }
    // }

    logFile.close();

    return 0;
}
