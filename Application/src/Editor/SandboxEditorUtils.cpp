#include "pch.h"
#include "SandboxEditorUtils.h"

#include <imgui/imgui.h>

namespace fs = std::filesystem;

namespace SandboxEditor
{
	bool ShowWarningWindow(std::string text, bool& yes)
	{
		bool clicked = false;
		ImGui::Begin("Warning");
		ImGui::Text(text.c_str());
		if (ImGui::Button("Yes"))
		{
			yes = true;
			clicked = true;
		}
		if (ImGui::Button("No"))
		{
			yes = false;
			clicked = true;
		}

		ImGui::SameLine();
		ImGui::End();
		return clicked;
	}

	void CreateFolder(std::filesystem::path directory, std::string folderName)
	{
		int i = 0;
		fs::path newFolder = directory;
		newFolder /= folderName;
		while (fs::exists(newFolder))
		{
			newFolder = directory;
			newFolder /= folderName + "_" + std::to_string(i++);
		}
		fs::create_directory(newFolder);
	}

	std::string FullDirectoryPath(std::filesystem::path path)
	{
		std::string fullPath = path.filename().string();
		while (path.has_parent_path())
		{
			std::string append = path.parent_path().filename().string() + "/";
			fullPath.insert(0, append.c_str());
			path = path.parent_path();
		}

		return fullPath;
	}
}