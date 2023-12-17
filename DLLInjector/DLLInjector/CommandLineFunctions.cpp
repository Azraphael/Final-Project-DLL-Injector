#include <Windows.h>
#include <TlHelp32.h>
#include <iostream>
#include <string>
#include <vector>
#include <filesystem>
#include <tchar.h>

//Our custom headers
#include "CommandeLineFunctions.h"
#include "systemfunctions.h"

/*
 * This file contains all command line functionality
 */


using namespace std;

/* Functions for Printing to Console*/
template<typename... Args>
void print_error(std::string_view message, Args&&... args)
{
	std::cout << "\033[31m";
	std::cout << vformat(message, make_format_args(args...));
}
template<typename... Args>
void print_good(std::string_view message, Args&&... args)
{
	std::cout << "\033[32m";
	std::cout << vformat(message, make_format_args(args...));
}
/**
 * \brief 
 * \param msg Print Error for ListProcess
 */

BOOL get_process_list()
{
	PROCESSENTRY32 pe32;

	// Take a snapshot of all processes in the system.
	const HANDLE h_process_snap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (h_process_snap == INVALID_HANDLE_VALUE)
	{
		print_error(" [-] CreateToolhelp32Snapshot (of processes)");
		return(FALSE);
	}

	// Set the size of the structure before using it.
	pe32.dwSize = sizeof(PROCESSENTRY32);

	// Retrieve information about the first process,
	// and exit if unsuccessful
	if (!Process32First(h_process_snap, &pe32))
	{
		//printError(TEXT("Process32First")); // show cause of failure
		CloseHandle(h_process_snap);          // clean the snapshot object
		return(FALSE);
	}

	// Now walk the snapshot of processes, and
	// display information about each process in turn
	do
	{
		if(pe32.szExeFile != nullptr)			
			_tprintf(TEXT("\nProcess: %s\n\tID: 0x%08X"), pe32.szExeFile, pe32.th32ProcessID);

	} while (Process32Next(h_process_snap, &pe32));

	CloseHandle(h_process_snap);
	return(TRUE);
}


/**
 * \brief Sets DLL name
 * \param pos Position of arguments that it will start in
 * \param argc Total arguments to parse through
 * \param args vector containing all arguments
 * \return 0
 */
int cmdline_set_dll(int pos, int argc, vector<string> args)
{
	//Copy Filename from Position + 1 in args, then return;
	if(argc >= pos+1)
		dll_file_name = args[pos + 1];

	return 0;
}
int cmdline_list_processes(int pos, int argc, vector<string> args)
{
	get_process_list();
    return 0;
}

int cmdline_show_help(int pos, int argc, vector<string> args)
{
    cout << "Command\t\t\tDescription\n\n";

    for (auto& a : *command_line_options)
    {
        cout << a.usage << a.description << '\n';
    }
    return 0;
}
int cmdline_set_process(int pos, int argc, std::vector<std::string> args)
{
	//Copy PRocess Name from Position + 1 in args, then return;
	if (argc >= pos + 1) {
		string processIdOrName = args[pos + 1];
		if (is_hex_notation(processIdOrName)) //It's an ID, use that instead.
			process_id = stoul(processIdOrName, nullptr, 16);
		else
			process_file_name = args[pos + 1];
	}
    return 0;
}
void program_info()
{
	std::cout << "DLL Injector - v1.0\n";
	std::cout << "Written By: Chad Stalcup\n";
}
