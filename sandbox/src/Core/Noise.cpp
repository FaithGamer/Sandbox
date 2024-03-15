#include "pch.h"
#include "Sandbox/Core/Noise.h"

namespace Sandbox
{
	Noise::Noise() : m_frequency(0.5f)
	{
	}
	float Noise::Get(float x, float y)
	{
		return m_generator.noise2D_01(x * m_frequency, y * m_frequency);
	}
	void Noise::SetFrequency(float frequency)
	{
		m_frequency = frequency;
	}
	void Noise::Seed(unsigned int seed)
	{
		m_generator.reseed(seed);
	}
	float Noise::Noise2D(float x, float y, float frequency)
	{
		static siv::PerlinNoise generator;
		return generator.noise2D_01(x * frequency, y * frequency);
	}

}
