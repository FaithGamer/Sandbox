#include "pch.h"

#include "Game.h"
#include <Sandbox/Render.h>
#include <Sandbox/Random.h>

using namespace Sandbox;



/* -- hero -- */

void HeroSystem::OnStart()
{
	//Controls

	auto inputMap = Inputs::GetInputMap("InputMap_0");
	//Input
	auto moveSprite = inputMap->CreateDirectionalInput("MoveSprite");
	std::vector<DirectionalButton> buttons
	{
		DirectionalButton(KeyScancode::W, Vec2f(0, 1)),
		DirectionalButton(KeyScancode::A, Vec2f(-1, 0)),
		DirectionalButton(KeyScancode::S, Vec2f(0, -1)),
		DirectionalButton(KeyScancode::D, Vec2f(1, 0)),
	};
	moveSprite->AddButtons(buttons);
	moveSprite->signal.AddListener(&HeroSystem::OnMove, this);

	//Instance Hero

	sptr<Texture> heroTexture = makesptr<Texture>("assets/textures/hero.png", 16.f,
		TextureImportSettings(TextureFiltering::Nearest, TextureWrapping::Clamp, true, false));
	sptr<Sprite> heroSprite = makesptr<Sprite>(heroTexture);

	auto world = Systems::GetMainWorld();
	auto hero = world->CreateEntity();
	hero->AddComponent<Hero>();
	hero->AddComponent<Transform>()->SetPosition(0, 0, 0);
	auto render = hero->AddComponent<SpriteRender>();
	render->SetSprite(heroSprite);
	render->SetLayer(Renderer2D::GetLayerId("640p"));
}

void HeroSystem::OnUpdate(Time delta)
{
	ForEachComponent<Hero, Transform>([delta](Hero& hero, Transform& transform) {
		float speed = 15.f;
		auto pos = transform.GetPosition();
		pos.x += hero.direction.x * (float)delta * speed;
		pos.y += hero.direction.y * (float)delta * speed;
		transform.SetPosition(pos);
		});
}

void HeroSystem::OnMove(Sandbox::InputSignal* input)
{
	ForEachComponent<Hero>([input](Hero& hero) {
		hero.direction = input->GetVec2f();
		});
}

/* -- enemy -- */

void EnemySystem::OnStart()
{
	sptr<Texture> enemyTexture = makesptr<Texture>("assets/textures/nice_guy.png", 16.f,
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
	float speed = 2.f;

	//Move towards hero
	ForEachComponent<Transform, MoveTowardsHero>([this, deltaTime, heroPos, speed](Transform& transform, MoveTowardsHero& mover)
		{
			Vec2f direction = heroPos - transform.GetPosition();
			direction = glm::normalize(direction) * speed;
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
	auto transform = entity->AddComponent<Transform>();
	float x = Random::Range(-16, 16);
	float y = Random::Range(-10, 10);
	transform->SetPosition({ x, y, 0 });
	entity->AddComponent<MoveTowardsHero>();
	auto render = entity->AddComponent<SpriteRender>();
	render->SetSprite(m_enemySprite);
	render->SetLayer(Renderer2D::GetLayerId("640p"));
}


