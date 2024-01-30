#include "pch.h"

#include "Game.h"
#include <Sandbox/Render.h>
#include <Sandbox/Random.h>
#include <cmath>

using namespace Sandbox;

/* -- hero -- */
sptr<Texture> LoadPixelArtTexture(std::string path)
{
	return makesptr<Texture>(path, 16.f,
		TextureImportSettings(TextureFiltering::Nearest, TextureWrapping::Clamp, true, false));
}
void HeroSystem::OnStart()
{
	LoadAssets();
	//Controls

	auto inputMap = Inputs::GetInputMap("InputMap_0");
	//Input
	auto moveInput = inputMap->CreateDirectionalInput("Move");
	std::vector<DirectionalButton> buttons
	{
		DirectionalButton(KeyScancode::W, Vec2f(0, 1)),
		DirectionalButton(KeyScancode::A, Vec2f(-1, 0)),
		DirectionalButton(KeyScancode::S, Vec2f(0, -1)),
		DirectionalButton(KeyScancode::D, Vec2f(1, 0)),
	};
	moveInput->AddButtons(buttons);
	moveInput->signal.AddListener(&HeroSystem::OnMove, this);

	auto fireInput = inputMap->CreateButtonInput("Fire");
	fireInput->AddMouse(MouseButton::Left);
	fireInput->signal.AddListener(&HeroSystem::OnFire, this);
	fireInput->SetSendSignalOnRelease(true);

	//Instance Hero

	sptr<Texture> heroTexture = LoadPixelArtTexture("assets/textures/hero.png");
	sptr<Sprite> heroSprite = makesptr<Sprite>(heroTexture);

	Entity hero;
	hero.AddComponent<Hero>();
	hero.AddComponent<Transform>()->SetPosition(0, 0, 0);
	auto render = hero.AddComponent<SpriteRender>();
	render->SetSprite(heroSprite);
	render->SetLayer(Renderer2D::GetLayerId("640p"));
}

void HeroSystem::OnUpdate(Time delta)
{
	auto world = Systems::GetMainWorld();
	//Move hero
	ForEachComponent<Hero, Transform>([delta, this](Hero& hero, Transform& transform) {
		float speed = 15.f;
		auto pos = transform.GetPosition();
		pos.x += hero.direction.x * (float)delta * speed;
		pos.y += hero.direction.y * (float)delta * speed;
		transform.SetPosition(pos);
		m_heroPosition = pos;
		hero.elapsed += delta;

		if (hero.isFiring && hero.elapsed >= hero.fireRate)
		{
			auto mouse = Systems::GetMainCamera()->ScreenToWorld(GetMousePosition(), Window::GetSize());
			InstantiateBullet(m_heroPosition, mouse);
			hero.elapsed = 0.f;
		}
		Systems::GetMainCamera()->SetPosition(pos);
		});

	//Move bullet
	std::vector<Entity> toDestroy;
	ForEachEntity<Bullet, Transform, CircleHitbox>([&toDestroy, this, delta, world]
	(Entity bulletEntt, Bullet& bullet, Transform& bulletTransform, CircleHitbox& bulletHitbox)
		{
			auto pos = bulletTransform.GetPosition();
			Vec2f offset{ bullet.direction.x * bullet.speed * (float)delta,
			bullet.direction.y * bullet.speed * (float)delta };
			pos.x += offset.x;
			pos.y += offset.y;
			bullet.distance += offset.Magnitude();
			bool collided = false;
			if (bullet.distance >= 100.f)
			{
				bulletEntt.Destroy();
			}
			else
			{
				bulletTransform.SetPosition(pos);
				ForEachEntity<Enemy, Transform, CircleHitbox>(
					[&collided, &toDestroy, bulletEntt, pos, world, bulletHitbox, this]
				(Entity enemyEntt, Enemy& enemy, Transform& enemyTransform, CircleHitbox& enemyHitbox)
					{
						//Circle collision
						if (!collided && pos.Distance(enemyTransform.GetPosition()) < enemyHitbox.radius + bulletHitbox.radius)
						{
							collided = true;
							InstanceBurstParticle(enemyTransform.GetPosition());
							//todo wrong position
							enemyEntt.Destroy();
							toDestroy.emplace_back(bulletEntt);
						}
					});
			}

		});
	for (auto entt : toDestroy)
	{
		entt.Destroy();
	}
}

void HeroSystem::OnFire(Sandbox::InputSignal* input)
{
	ForEachComponent<Hero>([input](Hero& hero) {
		hero.isFiring = input->GetBool();
		});
}

void HeroSystem::InstantiateBullet(Vec3f origin, Vec3f target)
{
	auto direction = (target - origin).Normalized();
	auto world = Systems::GetMainWorld();
	auto bullet = world->CreateEntity();
	bullet.AddComponent<Bullet>()->direction = (Vec2f)direction;
	bullet.AddComponent<CircleHitbox>()->radius = 0.5f;
	auto render = bullet.AddComponent<SpriteRender>();
	render->SetSprite(m_bulletSprite);
	render->SetLayer(Renderer2D::GetLayerId("Particles"));
	auto transform = bullet.AddComponent<Transform>();
	transform->SetPosition(origin);
	transform->SetRotationZAxis(glm::degrees(std::atan2(direction.x, direction.y)));
}

void HeroSystem::InstanceBurstParticle(Vec3f position)
{
	Entity particle;
	particle.AddComponent<Transform>()->SetPosition(position);
	auto part = particle.AddComponent<ParticleGenerator>();
	part->countByInstance = 60;
	part->particleFrequency = 0.09f;
	part->particleLifeTime = 0.4f;
	part->layer = Renderer2D::GetLayerId("Particles");
	part->duration = 0.1f;

}

void HeroSystem::OnMove(Sandbox::InputSignal* input)
{
	ForEachComponent<Hero>([input](Hero& hero) {
		hero.direction = input->GetVec2f();
		});
}

void HeroSystem::LoadAssets()
{
	auto bulletTexture = LoadPixelArtTexture("assets/textures/bullet.png");
	m_bulletSprite = makesptr<Sprite>(bulletTexture);
}

/* -- enemy -- */

void EnemySystem::OnStart()
{
	sptr<Texture> enemyTexture = makesptr<Texture>("assets/textures/pig.png", 16.f,
		TextureImportSettings(TextureFiltering::Nearest, TextureWrapping::Clamp, true, false));
	m_enemySprite = makesptr<Sprite>(enemyTexture);
}

void EnemySystem::OnUpdate(Time deltaTime)
{
	Hero* heroPtr;
	Vec3f heroPos;

	//Get Hero pos:
	ForEachComponent<Hero, Transform>([&heroPtr, &heroPos](Hero& hero, Transform& heroTransform) {
		heroPtr = &hero;
		heroPos = heroTransform.GetPosition();
		});
	float speed = 7.f;

	if (m_instanceClock.GetElapsed() > 1.f)
	{
		InstanceEnemy();
		m_instanceClock.Restart();
	}

	//Move towards hero
	ForEachComponent<Transform, Enemy>([this, deltaTime, heroPos, speed]
	(Transform& transform, Enemy& enemy)
		{
			enemy.timer += deltaTime;
			if (enemy.timer >= enemy.nextTime)
			{
				enemy.targetOffset.x = Random::Range(-2.f, 2.f);
				enemy.targetOffset.y = Random::Range(-2.f, 2.f);

				enemy.nextTime = Random::Range(1.f, 4.f);
				enemy.timer = 0;
			}
			Vec3f direction = heroPos - transform.GetPosition() + Vec3f{ enemy.targetOffset.x, enemy.targetOffset.y, 0 };
			direction.Normalize();
			auto position = transform.GetPosition();
			position.x += (float)deltaTime * direction.x * speed;
			position.y += (float)deltaTime * direction.y * speed;
			transform.SetPosition(position);
		});
}

void EnemySystem::InstanceEnemy()
{
	auto world = Systems::GetMainWorld();

	auto entity = world->CreateEntity();
	auto transform = entity.AddComponent<Transform>();
	float x = Random::Range(-40, 40);
	float y = Random::Range(-40, 40);
	transform->SetPosition({ x, y, 0 });
	entity.AddComponent<Enemy>();
	auto render = entity.AddComponent<SpriteRender>();
	render->SetSprite(m_enemySprite);
	render->SetLayer(Renderer2D::GetLayerId("640p"));
	entity.AddComponent<CircleHitbox>()->radius = 0.5f;

}


