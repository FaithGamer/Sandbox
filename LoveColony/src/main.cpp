#include "pch.h"
#include "Sandbox/Sandbox.h"

#include "Systems/GameManager.h"
#include "Systems/ColonistSystem.h"
#include "Systems/ZisYSystem.h"

using namespace Sandbox;

int main(int argv, char** argc)
{
	Engine::Init();

	Camera camera;
	camera.SetOrthographic(true);
	camera.worldToScreenRatio = 0.04f;
	Systems::SetMainCamera(&camera);
	Window::GetResizeSignal()->AddListener(&Camera::SetAspectRatio, &camera);

	//Physics layers
	Physics::AddLayer("Colonist");
	Physics::AddLayer("Scent");
	Physics::AddLayer("Walls");

	//Physics::DrawColliders(true);

	//Systems
	Systems::Push<ColonistSystem>();
	Systems::Push<ZisYSystem>();
	Systems::Push<GameManager>();


	Engine::Launch();

	return 0;
}