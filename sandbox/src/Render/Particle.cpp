#include "pch.h"
#include "Sandbox/Render/Particle.h"

namespace Sandbox
{
	ParticleGenerator::ParticleGenerator() : 
		duration(2), particleLifeTime(1), particleFrequency(0.3f), countByInstance(1),
		spreadAngle(360), speed(20.f), loop(false), destroyWhenOver(true), emitting(true), instancedCount(0), layer(0)
	{

		static auto texture = makesptr<Texture>("assets/textures/square.png", 16.f, 
			TextureImportSettings(TextureFiltering::Nearest, TextureWrapping::Clamp, true, false));
		sprite = makesptr<Sprite>(texture);
	}
	Particle::Particle() : SpriteRender()
	{
		m_layer = 0;
	}
}