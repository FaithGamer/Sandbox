#pragma once

#include <string>
#include <filesystem>

namespace SandboxEditor
{
	/// @brief Display a warning box with a message with Yes and No option.
	/// @param Message to display 
	/// @param yes capture the clicked option
	/// @return return true if clicked on yes or no 
	bool ShowWarningWindow(std::string text, bool& yes);
	void CreateFolder(std::filesystem::path path, std::string name);
	std::string FullDirectoryPath(std::filesystem::path path);
}

