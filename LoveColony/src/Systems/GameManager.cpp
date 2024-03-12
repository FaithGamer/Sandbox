#include "pch.h"
#include "GameManager.h"
#include "Prefab.h"
#include "Systems/ColonistSystem.h"
#include "Systems/ScentSystem.h"

GameManager::GameManager()
{
}

void GameManager::OnStart()
{
	StartGame();
	//Systems::Get<ScentSystem>()->DebugShowScent(true);
}

void GameManager::OnUpdate(Time delta)
{
}

void GameManager::StartGame()
{
	CreateMap();
	CreateEntities();
}

Entity CreateBox(Vec2f position, Vec2f dimensions)
{
	Entity box = Entity::Create();
	auto transform = box.AddComponent<Transform>();
	transform->SetPosition(position);
	auto body = box.AddComponent<StaticBody>(position, Physics::GetLayerMask("Walls"));
	body->SetLayerMask(Bitmask(333));
	body->AddCollider(makesptr<Box2D>(dimensions));

	return box;

}
int mapHeight = 20;
int mapWidth = 50;
Vec2f GridToWorld(int x, int y)
{
	return Vec2f(
		x - mapWidth * 0.5f,
		y - mapHeight * 0.5f
	);
}
void GameManager::CreateMap()
{
	//Create world edges
	CreateBox(Vec2f(25, 0), Vec2f(4, 40));
	CreateBox(Vec2f(-25, 0), Vec2f(4, 40));
	CreateBox(Vec2f(0, 14), Vec2f(50, 4));
	CreateBox(Vec2f(0, -14), Vec2f(50, 4));

	
	//Create walls

	float density = 0.3f;

	for (int y = 0; y < mapHeight; y++)
	{
		for (int x = 0; x < mapWidth; x++)
		{
			float noise = Noise::Noise2D((float)x, (float)y, 0.4);
			if (noise < density)
			{
				Entity wall = Prefab::Wall(GridToWorld(x, y));
			}
		}
	}
}

void GameManager::CreateEntities()
{
	for (int i = 0; i < 10000; i++)
	{
		Systems::Get<ColonistSystem>()->CreateColonist(ColonistInit(Vec2f(Random::Range(-25.f, 25.f), Random::Range(-10.f, 10.f))));
	}

	/*for (int i = 0; i <4000; i++)
	{
		auto pos = Vec2f(Random::Range(-25.f, 25.f), Random::Range(-10.f, 10.f));
		std::vector<OverlapResult> dummy;
		Systems::Get<ScentSystem>()->TryCreateTrackScent(ScentInit(Scent::Type::Food, pos, 0), dummy);
	}*/
}