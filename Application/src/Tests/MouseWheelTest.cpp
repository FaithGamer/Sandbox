#include "Sandbox/Sandbox.h"
#include "imgui/imgui.h"
using namespace Sandbox;


class Scroll : public System
{
public:
	void OnStart() override
	{
		auto inputs = Inputs::GetInputMaps()[0];
		inputs->GetInput("Scroll")->signal.AddListener(&Scroll::OnScroll, this);
		inputs->GetInput("Button")->signal.AddListener(&Scroll::OnButton, this);
	}
	void OnScroll(InputSignal* signal)
	{
		String value = std::to_string(signal->GetVec2f().y);
		LOG_INFO("scroll: " + value);
	}
	void OnImGui() override
	{
		ImGui::Begin("prout");
			ImGui::Text("zeafzrg");
			ImGui::Text("zeafzrg");
		ImGui::End();
	}
	void OnButton(InputSignal* signal)
	{
		LOG_INFO("button");
	}
};
void MouseWheelTest()
{
	Engine::Init();
	Camera cam;
	Systems::SetMainCamera(&cam);
	auto map = Inputs::CreateInputMap();
	auto input = map->CreateDirectionalInput("Scroll");
	map->CreateButtonInput("Button")->AddMouse(MouseButton::Left);
	input->SetMouseWheel(true);
	Systems::Push<Scroll>();
	Engine::Launch();
}