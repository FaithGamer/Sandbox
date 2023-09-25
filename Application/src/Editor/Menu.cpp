#include "pch.h"
#include "Menu.h"

namespace SandboxEditor
{
	Menu::Menu() : m_layout(nullptr)
	{

	}
	void Menu::Display(ImVec2& position, ImVec2& size)
	{
		if (ImGui::BeginMainMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				ImGui::Selectable("Dummy");
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Layout"))
			{
				if (ImGui::Selectable("Reset"))
				{
					m_layout->Reset();
				}
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

	void Menu::SetLayout(Layout* layout)
	{
		m_layout = layout;
	}
}