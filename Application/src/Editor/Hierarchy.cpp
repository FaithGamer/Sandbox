#include "pch.h"

#include "Hierarchy.h"

namespace SandboxEditor
{
	void Hierarchy::Display(ImVec2& position, ImVec2& size)
	{
		ImGui::SetNextWindowPos(position);
		ImGui::SetNextWindowSize(size);
		

		ImGui::Begin("Hierarchy");

		ImGui::Selectable("Dummy");

		ImGui::End();
	}
	std::string Hierarchy::GetName()
	{
		return "Hierarchy";
	}
}