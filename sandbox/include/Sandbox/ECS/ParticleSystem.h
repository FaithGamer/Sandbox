#pragma once

#include "Sandbox/ECS/System.h"
#include "Sandbox/Render/Particle.h"

namespace Sandbox
{
	class ParticleSystem : public System
	{
	public:
		ParticleSystem();
		void OnStart() override;
		void OnUpdate(Time deltaTime) override;
		void OnRender() override;
		int GetUsedMethod() override;
		void OnAddParticle(ComponentSignal componentSignal);
	private:
		void InstanceParticle(ParticleGenerator& generator, EntityId generatorId);
		std::vector<Particle> m_particles;
		std::vector<size_t> m_freeSpot;
		size_t m_lastParticle;
	};
}
