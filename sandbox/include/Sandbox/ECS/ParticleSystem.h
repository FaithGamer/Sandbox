#pragma once

#include "Sandbox/ECS/System.h"
#include "Sandbox/Render/Particle.h"

namespace Sandbox
{
	class ParticleSystem : public System
	{
	public:
		ParticleSystem();

		void OnUpdate(Time deltaTime) override;
		void OnRender() override;
		int GetUsedMethod() override;
	private:
		void InstanceParticle(ParticleGenerator& generator, Entity entity);
	};
}
