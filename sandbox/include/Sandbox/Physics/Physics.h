#pragma once

#include "Sandbox/Singleton.h"
#include "Sandbox/Physics/Raycast.h"
#include "Sandbox/Physics/Bitmask.h"
#include "Sandbox/Physics/Body.h"

namespace Sandbox
{
	/// @brief Everything for collision and physics.
	/// Currently doesn't take into consideration different worlds.
	class Physics : public Singleton<Physics>
	{
	public:
		Physics();
		~Physics();

		static void RaycastClosest(RaycastResult& result, Vec2f start, Vec2f end, Bitmask mask = 65535);
		static void RaycastAll(std::vector<RaycastResult>& result, Vec2f start, Vec2f end, Bitmask mask = 65535);
		static void CircleOverlap(std::vector<OverlapResult>& results, Vec2f pos, float radius, Bitmask mask = 65535);
		static void PointInside(std::vector<OverlapResult>& result, Vec2f pos, Bitmask mask = 65535);
		static float BodyDistance(Body& lhs, Body& rhs);
		static b2World* GetB2World();
	private:

		b2World* m_world;
		friend Singleton<Physics>;
	};
}