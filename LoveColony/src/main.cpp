#include "pch.h"
#include "Sandbox/Sandbox.h"

#include "Systems/GameManager.h"
#include "Systems/ColonistSystem.h"
#include "Systems/ScentSystem.h"
#include "Systems/ZisYSystem.h"

using namespace Sandbox;

struct NullEntity
{
	int tag;
};
int main(int argv, char** argc)
{
	Engine::Init();

	Camera camera;
	camera.SetOrthographic(true);
	camera.worldToScreenRatio = 0.04f;
	Systems::SetMainCamera(&camera);
	Window::GetResizeSignal()->AddListener(&Camera::SetAspectRatio, &camera);

	//Null entity
	Entity::Create().AddComponent<NullEntity>();

	//Physics layers
	Physics::AddLayer("Colonist");
	Physics::AddLayer("Scent");
	Physics::AddLayer("Walls");
	Physics::AddLayer("POI");

	//Physics::DrawColliders(true);

	//Render layers

	Renderer2D::AddLayer("Terrain");
	Renderer2D::AddLayer("Map");

	//Systems
	Systems::Push<ScentSystem>();
	Systems::Push<ColonistSystem>();
	Systems::Push<ZisYSystem>();
	Systems::Push<GameManager>();

	

	Engine::Launch();

	return 0;
}