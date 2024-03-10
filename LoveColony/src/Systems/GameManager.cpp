#include "pch.h"
#include "GameManager.h"
#include "Prefab.h"

GameManager::GameManager()
{
}

void GameManager::OnStart()
{
	StartGame();
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
	auto body = box.AddComponent<Body>(Body::Type::Kinematic, Physics::GetLayerMask("Walls"));
	body->AddCollider(makesptr<Box2D>(dimensions));

	return box;

}
void GameManager::CreateMap()
{
	//Create walls
	CreateBox(Vec2f(25, 0), Vec2f(4, 40));
	CreateBox(Vec2f(-25, 0), Vec2f(4, 40));
	CreateBox(Vec2f(0, 14), Vec2f(50, 4));
	CreateBox(Vec2f(0, -14), Vec2f(50, 4));
}

void GameManager::CreateEntities()
{
	for (int i = 0; i < 1000; i++)
	{
		Prefab::Colonist();
	}
}