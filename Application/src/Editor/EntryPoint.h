#pragma once

#include "Sandbox/Sandbox.h"
#include "Sandbox/ECS.h"
#include "Sandbox/Render/Camera.h"
#include "Sandbox/Render/Window.h"
#include "Sandbox/Render/Renderer2D.h"
#include "EditorSystem.h"
#include "Game.h"

using namespace Sandbox;

void Editor()
{
	//Initialize
	Engine::Init();

	//World
	Systems::CreateWorld();

	//Systems
	Systems::Push<EnemySystem>();
	Systems::Push<HeroSystem>();
	Systems::Push<SandboxEditor::EditorSystem>();

	//Camera
	Camera cam;
	cam.worldToScreenRatio = 0.05f;
	cam.SetOrthographic(true);
	cam.SetAspectRatio(1600.f / 900.f);
	Window::GetResizeSignal()->AddListener(&Camera::SetAspectRatio, &cam);
	Systems::SetMainCamera(&cam);

	//Add a custom rendering layer with a fixed height
	Renderer2D::AddLayer("Particles", 640);
	Renderer2D::AddLayer("640p", 640);

	//Input Map
	Inputs::CreateInputMap();

	//Start
	Engine::Launch();
}
