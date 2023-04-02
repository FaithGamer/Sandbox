#include "ImGuiSystemTest.h"
#include "imgui/imgui.h"

void ImGuiSystemTest::OnImGui()
{
	bool show_demo_window = true;
	ImGui::ShowDemoWindow(&show_demo_window);
}