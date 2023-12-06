// DLLInjector.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <string>
#include <vector>
using namespace std;


int showHelp(int, int, char*);
int setDll(int, int, char*);
int listProcesses(int, int, char*);
/**
 * \brief This will allow us to write out each command line item and be reusable code and for easier readability
 */
struct argument_commands {
    string command;
    string usage;
    string description;
    int (*func)(int,int, vector<string>);    
};


vector<argument_commands> commandLineOptions = {}; //Can't set anything here, just initialize and set later.




int setDll(int pos, int argc, vector<string> args)
{
    return 0;
}
int listProcesses(int pos, int argc, vector<string> args)
{
    return 0;
}

int showHelp(int pos, int argc, vector<string> args)
{
    cout << "COMMAND\t\tUSAGE\t\tDESCRIPTION\n\n";
    
    for (auto &a: commandLineOptions)
    {
        cout << a.command << "\t\t" << a.usage << "\t\t" << a.description << '\n';
    }
}
void programInfo()
{
    cout << "DLL Injector - v1.0\n";
    cout << "Written By: Chad Stalcup\n";
}


int main(int argc, char* argv[])
{
    vector<argument_commands> cmd = {
    { "-h","-h", "Lists all arguments available", &showHelp},
    { "-L", "-L", "List all processes", &listProcesses},
    { "-d", "-d <dll-to-inject>","Select DLL to Inject", &setDll}
    };

    std::copy(cmd.begin(), cmd.end(), back_inserter(cmd));
    vector<std::string> all_args;

    if (argc > 1) {
        all_args.assign(argv + 1, argv + argc);
    }

    programInfo(); // Output program details initially
    if (all_args.empty())
        commandLineOptions[0].func(0, 0, all_args);
        return 0;

    for(int i=0; i<argc; i++)
    {
	    
    }
    
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
