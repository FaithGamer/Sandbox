#include "pch.h"
#include "Sandbox/ECS/ParticleSystem.h"
#include "Sandbox/Render/Particle.h"
#include "Sandbox/Render/Renderer2D.h"
#include "Sandbox/Random.h"
#include "Sandbox/Math.h"
#include "Sandbox/ECS/Systems.h"

#define MAX_PARTICLES 50000

namespace Sandbox
{
	ParticleSystem::ParticleSystem()
	{
		SetPriority(9999);
	}

	void ParticleSystem::OnStart()
	{
		Systems::GetMainWorld()->ListenOnAddComponent<ParticleGenerator>(&ParticleSystem::OnAddParticle, this);
		m_particles.resize(MAX_PARTICLES);
		m_freeSpot.emplace_back(0);
		LOG_INFO("Particle System size, for {0} particles max: {1} ko", MAX_PARTICLES, (float)sizeof(Particle) * MAX_PARTICLES / 1000);
	}

	void ParticleSystem::OnUpdate(Time deltaTime)
	{
		ForEachEntity<ParticleGenerator>([this, deltaTime](Entity entity, ParticleGenerator& generator) {

			if (generator.internalClock <= 0.f)
				generator.internalClock = generator.particleFrequency;
			else
				generator.internalClock += deltaTime;

			if (!generator.emitting)
				return;

			if (generator.internalClock >= generator.duration)
			{
				if (generator.loop)
				{
					generator.internalClock = 0;
					generator.instancedCount = 0;
					generator.deadCount = 0;
				}
				else
				{
					if (generator.destroyWhenOver && generator.deadCount >= generator.instancedCount)
						entity.Destroy();

					generator.emitting = false;
					return;
				}
			}
			int count = floor((float)generator.internalClock / (float)generator.particleFrequency);
			for (int i = 0; i < count * generator.countByInstance - generator.instancedCount; i++)
			{
				InstanceParticle(generator, entity.GetId());
			}
			});

		auto& registry = Systems::GetMainWorld()->registry;

		registry.view<Particle, Transform>().each(

			[&](EntityId entt, Particle& particle, Transform& transform)
			{
				particle.internalClock += deltaTime;

				if (particle.internalClock >= particle.lifeTime)
				{
					auto generator = registry.try_get<ParticleGenerator>(particle.generator); 
					if(generator)
						generator->deadCount++;
					registry.destroy(entt);

				}

				auto offset = particle.velocity * (float)deltaTime;
				transform.Move(offset);
			});

	}

	void ParticleSystem::OnRender()
	{
		auto& registry = Systems::GetMainWorld()->registry;
		auto renderer = Renderer2D::Instance();

		ForEachEntity<Particle, Transform>([&registry, this, renderer](Entity entity, Particle& particle, Transform& transform)
			{
				if (particle.needUpdateRenderBatch)
				{
					particle.renderBatch = Renderer2D::GetBatchId(particle.GetLayer(), particle.GetShader());
					particle.needUpdateRenderBatch = false;
				}
				auto generatorTransform = registry.try_get<Transform>(particle.generator);
				if (!generatorTransform)
					entity.Destroy();
				else
				{
					Transform trans;
					trans.SetPosition(transform.GetPosition() + generatorTransform->GetPosition());
					renderer->DrawSprite(trans, particle, particle.renderBatch);
				}
			});
	}

	int ParticleSystem::GetUsedMethod()
	{
		return Method::Render | Method::Update;
	}

	void ParticleSystem::OnAddParticle(ComponentSignal componentSignal)
	{
		auto generator = Entity(componentSignal.entity).GetComponent<ParticleGenerator>();

	}

	void ParticleSystem::InstanceParticle(ParticleGenerator& generator, EntityId generatorId)
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
		particle.generator = generatorId;
		generator.instancedCount++;
	}

}