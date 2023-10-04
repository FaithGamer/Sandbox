#pragma once

#include "Sandbox/Time.h"
#include "Sandbox/Render/Sprite.h"
#include "Sandbox/Render/Transform.h"
#include "Sandbox/Render/SpriteRender.h"
#include "Sandbox/ECS/Entity.h"

namespace Sandbox
{
	class Particle : public SpriteRender
	{
	public:
		Particle();
		EntityId generator;
		Vec3f velocity = { 0, 0, 0 };

		Time internalClock;
		Time lifeTime;
		bool alive = false;
		
	};

	class ParticleGenerator 
	{
	public:
		ParticleGenerator();

		Time duration;
		Time particleLifeTime;
		Time particleFrequency;
		Time internalClock;

		unsigned int countByInstance;
		float spreadAngle;
		float speed;

		bool loop;
		bool destroyWhenOver;
		bool emitting;

		sptr<Sprite> sprite;
		uint32_t layer;
		sptr<Shader> shader;

		unsigned int instancedCount;
		unsigned int deadCount;
	private:

	};
}