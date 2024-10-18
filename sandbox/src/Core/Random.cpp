#include "pch.h"
#include "Sandbox/Core/Random.h"

namespace Sandbox
{
	namespace Random
	{
		static std::default_random_engine generator;
		void Seed(unsigned int seed)
		{
			generator.seed(seed);
		}
		int Range(int min, int max)
		{
			//static std::default_random_engine generator;
			
			std::uniform_int_distribution<int> dist(min, max);
			return dist(generator);
		}

		uint64_t Range(uint64_t min, uint64_t max)
		{
			//static std::default_random_engine generator;

			std::uniform_int_distribution<uint64_t> dist(min, max);
			return dist(generator);
		}

		int64_t Range(int64_t min, int64_t max)
		{
			//static std::default_random_engine generator;

			std::uniform_int_distribution<int64_t> dist(min, max);
			return dist(generator);
		}

		float Range(float min, float max)
		{
			//static std::default_random_engine generator;
			std::uniform_int_distribution<int64_t> dist(min * 10000, max * 10000);
			return (float)dist(generator) * 0.0001f;
		}
	}
}