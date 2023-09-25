#pragma once

#include "Sandbox/ECS/System.h"

class ImGuiSystem : public Sandbox::System
{
	void OnImGui();
};

void ImGuiSystemTest();