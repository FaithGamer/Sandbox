#include "pch.h"
#include "Sandbox/Core/Noise.h"

namespace Sandbox
{
	namespace Noise
	{
		float Noise2D(float x, float y, float frequency)
		{
			static siv::PerlinNoise generator;
			return generator.noise2D_01(x*frequency, y*frequency);
		}
	}
}
