// DLLInjector.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <codecvt>
#include <Windows.h>
#include <TlHelp32.h>
#include <iostream>
#include <string>
#include <vector>
#include <filesystem>
#include <cstring>
#include <tchar.h>
#include "CommandeLineFunctions.h"
#include "systemfunctions.h";

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
template<typename... Args>
void print_blue(std::string_view message, Args&&... args)
{
	std::cout << "\033[34m";
	std::cout << vformat(message, make_format_args(args...));
}
template<typename... Args>
void print_normal(std::string_view message, Args&&... args)
{
	std::cout << "\033[34m";
	std::cout << vformat(message, make_format_args(args...));
}
/**
 * \brief Starts a new process with a given process name
 * \param process_name Name of Process to Run new
 * \param prepend_chars Directory of File - Default Windows\System32
 * \return ProcessID
 */
int start_new_process(const char* process_name, const char* prepend_chars = R"(C:\Windows\System32\)")
{
	const size_t original_length = strlen(process_name);
	const size_t prepend_length = strlen(prepend_chars);
	const auto modified_name = new char[prepend_length + original_length + 1];
	strcpy_s(modified_name, prepend_length + original_length + 1, prepend_chars);
	strcat_s(modified_name, prepend_length + original_length + 1, process_name);

	const int wide_length = MultiByteToWideChar(CP_UTF8, 0, modified_name, -1, nullptr, 0);
 	const auto wide_process_name = new wchar_t[wide_length];
	MultiByteToWideChar(CP_UTF8, 0, modified_name, -1, wide_process_name, wide_length);

	STARTUPINFO si = {sizeof(si)};
	PROCESS_INFORMATION pi;

	const LPCTSTR application_name = wide_process_name;
	const LPTSTR command_line = nullptr;
	const LPSECURITY_ATTRIBUTES process_attributes = nullptr;
	const LPSECURITY_ATTRIBUTES thread_attributes = nullptr;
	constexpr BOOL inherit_handles = FALSE;
	constexpr DWORD creation_flags = 0;
	const LPVOID environment = nullptr;
	const LPCTSTR current_directory = nullptr;
	const LPSTARTUPINFO startup_info = &si;
	const LPPROCESS_INFORMATION process_information = &pi;

	if (CreateProcess(application_name, command_line, process_attributes,
	                  thread_attributes, inherit_handles, creation_flags, environment,
	                  current_directory, startup_info, process_information))
	{
		print_good("[+] {} process created successfully!\n", process_name);
		print_good("[+] Process ID: {}\n", pi.dwProcessId);
		

		//CloseHandle(pi.hProcess);
		//CloseHandle(pi.hThread);
		return static_cast<int>(pi.dwProcessId);
	}
	print_error(" [-] Failed to create \'{}\' process. Error code: {}\n", process_name, GetLastError());
	return 0;
}

/**
 * \brief Find's the process by name
 * \param name Process to Find
 * \return Process ID
 */
DWORD get_process_id_by_name(const char* name)
{
	PROCESSENTRY32 entry;
	entry.dwSize = sizeof(PROCESSENTRY32);
	char buf[MAX_PATH] = {0};
	size_t chars_converted = 0;

	const HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);

	if (Process32First(snapshot, &entry) == TRUE)
	{
		while (Process32Next(snapshot, &entry) == TRUE)
		{
			wcstombs_s(&chars_converted, buf, entry.szExeFile, MAX_PATH);
			if (_stricmp(buf, name) == 0)
			{
				const HANDLE ph = OpenProcess(PROCESS_ALL_ACCESS, FALSE, entry.th32ProcessID);
				if (ph != nullptr)
				{
					print_good(" [+] Got handle on {}\n", entry.th32ProcessID);					
					CloseHandle(snapshot);
					return entry.th32ProcessID;
				}
				print_blue(" [*] Could not obtain handle on {}.  Continuing...\n",entry.th32ProcessID);
			}
		}
	}
	CloseHandle(snapshot);
	return NULL;
}

/**
 * \brief Main Function
 * \param argc Total amount of arguments, argc=1+ (first variable is always the executable name)
 * \param argv Array of command line options
 * \return Returns exit code
 */
int main(int argc, char* argv[])
{
	const vector<argument_commands> cmd = {
		{"-h", "-H", "\t\t\tLists all arguments available", &cmdline_show_help},
		{"-L", "-L", "\t\t\tList all processes", &cmdline_list_processes},
		{"-d", "-d <dll-file>", "\t\tSelect DLL to Inject", &cmdline_set_dll},
		{"-p", "-p <process-name>", "\t\tSelect Process to Inject", &cmdline_set_process}
	};

	command_line_options = new vector<argument_commands>(cmd);
	vector<std::string> all_args;

	program_info(); // Output program details initially

	if (argc > 1)
	{
		all_args.assign(argv + 1, argv + argc);
	}


	if (all_args.empty()) //No command line options, show help screen
		command_line_options->at(0).func(0, argc, all_args);

	else
	{
		//There are some available process them
		for (int i = 0; i < all_args.size(); i++)
		{
			for (const auto& command : *command_line_options)
			{
			
			if (command.command.find(all_args[i]) == 0)
				command.func(i, argc - 1, all_args);  //Run each command
			}
			
		}
	}


	if (!process_file_name.empty()) //We have a file to run, continue into it
	{
		const string file = get_file_name(process_file_name);
		const string dir = get_path(process_file_name);
		DWORD pid = process_id == 0? get_process_id_by_name(file.c_str()) : process_id;

		if (pid == 0)
		{
			print_blue(" [*] Starting new \'{}\' process...\n", process_file_name);
			
			if (!dir.empty())
				pid = start_new_process(file.c_str(), dir.c_str()); //Start new process with directory if it exists in argument
			else
				pid = start_new_process(file.c_str()); //Start new process with default directory

			if (pid == 0) //Still not getting anything
			{
				print_error(" [-] Error with creating a new process, exiting with code: {}\n", GetLastError());				
				return 0;
			}
		} 
		

		if (!dll_file_name.empty()) //DLL file is selected, lets inject!
		{
			if (get_path(dll_file_name).empty())  //Path is empty so must be same path.
				dll_file_name.insert(0, ExePath() + "\\"); //Add a the current path to the dll path. 

			const auto w_dll = dll_file_name;

			const HMODULE h_kernel32 = GetModuleHandle(L"Kernel32");
			VOID* lb = GetProcAddress(h_kernel32, "LoadLibraryA");
			const HANDLE h_process = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
			const LPVOID alloc = VirtualAllocEx(h_process, nullptr, w_dll.length() + 1, (MEM_RESERVE | MEM_COMMIT),
				PAGE_EXECUTE_READWRITE);

			if (WriteProcessMemory(h_process, alloc, w_dll.c_str(), w_dll.length() + 1, nullptr)) {

				HANDLE th = CreateRemoteThread(h_process, nullptr, 0, static_cast<LPTHREAD_START_ROUTINE>(lb), alloc, 0,
					nullptr);

				if (th != nullptr)  //Check if thread exists, if so, display..
				{
					print_good(" [+] Successfully attached to process\n");
				}
				else
				{
					print_error(" [-] Unable to attach dll!\n");
				}
			} else
			{
				print_error(" [-] Unable to Write To Memory!\n");
			}
		}
	}
	else
	{
		if (!dll_file_name.empty()) {
			print_error(" [-] Must have a Process to inject into!");
			return 1;
		}
	}

	return 0;
}
