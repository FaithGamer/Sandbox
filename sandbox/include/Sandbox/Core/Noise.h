#pragma once

#include <PerlinNoise/PerlinNoise.hpp>

namespace Sandbox
{
	class Noise
	{
	public:
		Noise();
		/// @brief Get noise value for this coordinate
		/// @return noise value between 0 and 1
		float Get(float x, float y);
		/// @brief Set the noise frequency
		/// @param frequency range between 0 (big dense blocks) and 1 (little sparse blocks)
		void SetFrequency(float frequency);
		/// @brief Set the seed for RNG
		void Seed(unsigned int seed);
		/// @brief Generate noise with a static generator
		/// @return noise value between 0 and 1
		static float Noise2D(float x, float y, float frequency);
	private:
		siv::PerlinNoise m_generator;
		float m_frequency;
	};
}
