#include "pch.h"
#include "Menu.h"

namespace SandboxEditor
{
	void Menu::Display(ImVec2& position, ImVec2& size)
	{
		if (ImGui::BeginMainMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				ImGui::Selectable("Dummy");
				ImGui::EndMenu();
			}
			size = ImGui::GetWindowSize();

			ImGui::EndMainMenuBar();
		}

	}

	std::string Menu::GetName()
	{
		return "Menu";
	}
}