#pragma once
#include <string>
#include <vector>



/**
 * \brief This will allow us to write out each command line item and be reusable code for easier readability
 */
struct argument_commands {
	std::string command;
	std::string usage;
    std::string description;
    int (*func)(int, int, std::vector<std::string>);
};

std::vector<argument_commands>* command_line_options; //Can't set anything here, just initialize and set later.


std::string dll_file_name; //made global for use by command line functions
std::string process_file_name; //made global for use by command line functions
long process_id;

//All our command line function parsers. Proto-functions
int cmdline_show_help(int, int, std::vector<std::string>);
int cmdline_set_dll(int, int, std::vector<std::string>);
int cmdline_list_processes(int, int, std::vector<std::string>);
int cmdline_set_process(int, int, std::vector<std::string>);


void program_info();


