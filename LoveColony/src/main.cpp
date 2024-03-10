#include "pch.h"
#include "Sandbox/Sandbox.h"

#include "GameManager.h"
#include "ColonistSystem.h"

using namespace Sandbox;

int main(int argv, char** argc)
{
	Engine::Init();

	Camera camera;
	camera.SetOrthographic(true);
	Systems::SetMainCamera(&camera);

	//Physics layers
	Physics::AddLayer("Colonist");


	//Systems
	Systems::Push<GameManager>();
	Systems::Push<ColonistSystem>();

	Engine::Launch();

	return 0;
}