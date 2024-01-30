#include "pch.h"
#include "Sandbox/Random.h"

namespace Sandbox
{
	namespace Random
	{
		int Range(int min, int max)
		{
			static std::default_random_engine generator;
			std::uniform_int_distribution<int> dist(min, max);
			return dist(generator);
		}

		float Range(float min, float max)
		{
			static std::default_random_engine generator;
			std::uniform_int_distribution<int64_t> dist(min * 10000, max * 10000);
			return (float)dist(generator) * 0.0001f;
		}
	}
}