#include "common.hpp"
#include "functions.hpp"

int main(int argc, char **argv)
{
    initGlobalVariables();

    while (true)
    {
        string userInput = "";
        cout << "> ";
        getline(cin, userInput);
        auto inputSeparatedBySpace = splitString(userInput, " ");
        // for (auto s : inputSeparatedBySpace)
        // {
        //     cout << s << endl;
        // }
        string command = inputSeparatedBySpace.at(0);
        string arg = inputSeparatedBySpace.size() >= 2 ? inputSeparatedBySpace.at(1) : "";
        if (command == "exit")
        {
            cout << "Bye!" << endl;
            break;
        }
        if (commands.find(command) != commands.end())
        {
            FunctionVariant &funcVar = commands[command];
            if (holds_alternative<function<void()>>(funcVar) && arg.empty())
            {
                auto &func = get<function<void()>>(funcVar);
                func();
            }
            else if (holds_alternative<function<void(string)>>(funcVar) && !arg.empty())
            {
                auto &func = get<function<void(string)>>(funcVar);
                func(arg);
            }
            else
            {
                cout << "Command argument is invalid." << endl;
            }
        }
        else
        {
            cerr << "Unavailable command '" << command << "' was detected." << endl;
            cerr << "To see how to use, put 'help' as an command." << endl;
        }
    }

    return 0;
}
