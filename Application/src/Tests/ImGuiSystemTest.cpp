#include "ImGuiSystemTest.h"
#include "imgui/imgui.h"
#include "Sandbox/Engine.h"
#include "Sandbox/ECS.h"
#include "Sandbox/Render.h"
void ImGuiSystem::OnImGui()
{
	bool show_demo_window = true;
	ImGui::ShowDemoWindow(&show_demo_window);
}

using namespace Sandbox;

void ImGuiSystemTest()
{

	Engine::Init();
	Camera camera;
	Systems::SetMainCamera(&camera);
	Systems::Push<ImGuiSystem>();
	Engine::Launch();
}
