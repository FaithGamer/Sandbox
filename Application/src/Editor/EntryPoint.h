#pragma once

#include "Sandbox/ECS.h"
#include "Sandbox/Render/Camera.h"
#include "Sandbox/Render/Window.h"
#include "EditorSystem.h"

using namespace Sandbox;

void Editor()
{
	EngineParameters params;
	params.startupWindowResolution = { 320, 180 };
	Engine::Init(params);
	Camera cam;
	cam.SetOrthographic(true);
	Window::GetResizeSignal()->AddListener(&Camera::SetAspectRatio, &cam);
	Systems::SetMainCamera(&cam);
	Systems::CreateWorld();
	Systems::Push<SandboxEditor::EditorSystem>();
	
	Engine::Launch();
}
