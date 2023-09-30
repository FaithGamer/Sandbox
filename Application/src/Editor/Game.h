#pragma once

#include <Sandbox/ECS.h>
#include <Sandbox/Time.h>
#include <Sandbox/Input.h>
#include <Sandbox/Render/Sprite.h>

struct Hero
{
	Sandbox::Vec2f direction = { 0, 0 };
};

struct MoveTowardsHero
{
	float dummy;
};

class HeroSystem : public Sandbox::System
{
public:
	void OnStart();
	void OnUpdate(Sandbox::Time delta);
	void OnMove(Sandbox::InputSignal* input);
};

class EnemySystem : public Sandbox::System
{
public:
	void OnStart();
	void OnUpdate(Sandbox::Time delta);
	void InstanceEnemy();
private:
	sptr<Sandbox::Sprite> m_enemySprite;

};