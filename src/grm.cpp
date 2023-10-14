#include "common.hpp"
#include "functions.hpp"

int main(int argc, char **argv)
{
    initGlobalVariables();

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

    if (givenOption == "--help")
    {
        help();
    }
    else if (givenOption == "scan")
    {
        scan(argv[2]);
    }
    else if (givenOption == "check")
    {
        check();
    }
    else if (givenOption == "list")
    {
        list();
    }

    return 0;
}
