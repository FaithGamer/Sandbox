#include "pch.h"
#include "Sandbox/ECS/ParticleSystem.h"
#include "Sandbox/Render/Particle.h"
#include "Sandbox/Render/Renderer2D.h"
#include "Sandbox/Random.h"
#include "Sandbox/Math.h"
#include "Sandbox/ECS/Systems.h"

namespace Sandbox
{
	ParticleSystem::ParticleSystem()
	{
		SetPriority(9999);
	}

	void ParticleSystem::OnUpdate(Time deltaTime)
	{
		ForEachEntity<ParticleGenerator>([this, deltaTime](Entity entity, ParticleGenerator& generator) {

			generator.internalClock += deltaTime;

			if (!generator.emitting)
				return;

			if (generator.internalClock >= generator.duration)
			{
				if (generator.loop)
				{
					generator.internalClock = 0;
					generator.instancedCount = 0;
				}
				else
				{
					if (generator.destroyWhenOver)
						entity.Destroy();

					generator.emitting = false;
					return;
				}
			}
			int count = floor((float)generator.internalClock / (float)generator.particleFrequency * generator.countByInstance);
			for (int i = 0; i < count - generator.instancedCount; i++)
			{
				InstanceParticle(generator, entity);
			}
			});

		auto& registry = Systems::GetMainWorld()->registry;

		registry.view<Particle, Transform>().each(

			[&](EntityId entt, Particle& particle, Transform& transform)
			{
				particle.internalClock += deltaTime;

				if (particle.internalClock >= particle.lifeTime)
					registry.destroy(entt);

				auto offset = particle.velocity * (float)deltaTime;
				transform.Move(offset);
			});

	}

	void ParticleSystem::OnRender()
	{
		auto& registry = Systems::GetMainWorld()->registry;
		auto renderer = Renderer2D::Instance();

		ForEachComponent<Particle, Transform>([this, renderer](Particle& particle, Transform& transform)
			{
				if (particle.needUpdateRenderBatch)
				{
					particle.renderBatch = Renderer2D::GetBatchId(particle.GetLayer(), particle.GetShader());
					particle.needUpdateRenderBatch = false;
				}
				auto generatorTransform = particle.generator.GetComponent<Transform>();
				Transform finalTransform = transform;// +*generatorTransform;
				renderer->DrawSprite(finalTransform, particle, particle.renderBatch);
			});



	}

	int ParticleSystem::GetUsedMethod()
	{
		return Method::Render | Method::Update;
	}

	void ParticleSystem::InstanceParticle(ParticleGenerator& generator, Entity genEntity)
	{
		auto& registry = Systems::GetMainWorld()->registry;

		EntityId particleId = registry.create();
		Particle& particle = registry.emplace<Particle>(particleId);
		registry.emplace<Transform>(particleId);

		float angle = Random::Range(0.f, generator.spreadAngle);
		Vec2f direction = Math::AngleToVec(angle);
		particle.velocity = { direction.x * generator.speed, direction.y * generator.speed, 0.f };
		particle.lifeTime = generator.particleLifeTime;
		particle.SetSprite(generator.sprite);
		particle.SetLayer(generator.layer);
		particle.SetShader(generator.shader);
		particle.generator = genEntity;
	}

}