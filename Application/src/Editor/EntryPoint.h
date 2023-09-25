#pragma once

#include "Sandbox/ECS.h"
#include "Sandbox/Render/Camera.h"
#include "EditorSystem.h"

using namespace Sandbox;

void Editor()
{
	Engine::Init();
	Camera cam;
	Systems::SetMainCamera(&cam);
	Systems::CreateWorld();
	Systems::Push<SandboxEditor::EditorSystem>();
	
	Engine::Launch();
}
