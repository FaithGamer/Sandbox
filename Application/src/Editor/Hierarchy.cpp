#include "pch.h"
#include <Sandbox/ECS.h>
#include "Hierarchy.h"


using namespace Sandbox;

namespace SandboxEditor
{
	void Hierarchy::Display(ImVec2& position, ImVec2& size)
	{
		ImGui::SetNextWindowPos(position);
		ImGui::SetNextWindowSize(size);
		

		ImGui::Begin("Hierarchy");


		auto world = Systems::GetMainWorld();
		if (world == nullptr)
			ImGui::Text("No World instanced.");
		else
		{
			world->m_registry.each([](EntityId entity) {
				ImGui::Text(std::to_string((int)entity).c_str());
				});
		}


		position = ImGui::GetWindowPos();
		size = ImGui::GetWindowSize();

		ImGui::End();


	}
	std::string Hierarchy::GetName()
	{
		return "Hierarchy";
	}
}