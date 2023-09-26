#pragma once

#include "Sandbox/ECS.h"
#include "Sandbox/Render/Camera.h"
#include "Sandbox/Render/Window.h"
#include "EditorSystem.h"

using namespace Sandbox;

void Editor()
{
	Engine::Init();
	Camera cam;
	cam.SetOrthographic(true);
	Window::GetResizeSignal()->AddListener(&Camera::SetAspectRatio, &cam);
	Systems::SetMainCamera(&cam);
	Systems::CreateWorld();
	Systems::Push<SandboxEditor::EditorSystem>();
	
	Engine::Launch();
}
