#pragma once

#include <Sandbox/ECS.h>
#include <Sandbox/Time.h>
#include <Sandbox/Input.h>
#include <Sandbox/Render/Sprite.h>


struct Hero
{
	Sandbox::Vec2f direction = { 0, 0 };
	bool isFiring = false;
	Sandbox::Time fireRate = 0.02f;
	Sandbox::Time elapsed = fireRate;
};

struct Enemy
{
	float dummy;
};

struct CircleHitbox
{
	float radius = 0;
};

struct Bullet
{
	float speed = 50.f;
	Sandbox::Vec2f direction = { 0, 0 };
	float distance = 0.f;
};

class HeroSystem : public Sandbox::System
{
public:
	void OnStart();
	void OnUpdate(Sandbox::Time delta);
	void OnMove(Sandbox::InputSignal* input);
	void OnFire(Sandbox::InputSignal* input);

	void InstantiateBullet(Sandbox::Vec3f origin, Sandbox::Vec3f target);
	void InstanceBurstParticle(Sandbox::Vec3f position);
private:
	void LoadAssets();
	sptr <Sandbox::Sprite> m_bulletSprite;
	Sandbox::Vec3f m_heroPosition;
};

class EnemySystem : public Sandbox::System
{
public:
	void OnStart();
	void OnUpdate(Sandbox::Time delta);
	void InstanceEnemy();

private:
	sptr<Sandbox::Sprite> m_enemySprite;
	sptr<Sandbox::Sprite> m_particleSprite;
	Sandbox::Clock m_instanceClock;
};

