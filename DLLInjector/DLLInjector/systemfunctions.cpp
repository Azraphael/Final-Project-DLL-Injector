
#include <string>
#include <filesystem>
#include "systemfunctions.h"
#include <format>
#include <iostream>
#include <string>
#include <string_view>
#include <iostream>
#include <Windows.h>
#include <TlHelp32.h>

namespace fs = std::filesystem;




/**
 * \brief Gets the path from a string (treated as a file)
 * \param file File to check for path
 * \return path without ending "\"
 */
std::string get_path(const std::string file)
{
	return fs::path(file).parent_path().string();
}

/**
 * \brief Inputs a FQN and returns the filename
 * \param file File to get file_name from fully-qualified-name
 * \return filename without path (string)
 */
std::string get_file_name(const std::string file)
{
	return fs::path(file).filename().string(); //Returns just the filename
}
/**
 * \brief Checks if a string is in hex notation.
 * \param s String to be checked for hex notation (0x00FF)
 * \return true, if in hex notation
 */
bool is_hex_notation(std::string const& s)
{
	return s.compare(0, 2, "0x") == 0
		&& s.size() > 2
		&& s.find_first_not_of("0123456789abcdefABCDEF", 2) == std::string::npos;
}



std::string ExePath() {
	TCHAR buffer[MAX_PATH] = { 0 };
	GetModuleFileName(nullptr, buffer, MAX_PATH);
	const std::wstring::size_type pos = std::wstring(buffer).find_last_of(L"\\/");

	const std::wstring a = std::wstring(buffer).substr(0, pos);
	const int size_needed = WideCharToMultiByte(CP_UTF8, 0, &a[0], a.size(), nullptr, 0, nullptr, nullptr);
	std::string str_to(size_needed, 0);
	WideCharToMultiByte(CP_UTF8, 0, &a[0], a.size(), &str_to[0], size_needed, nullptr, nullptr);
	return str_to;
}