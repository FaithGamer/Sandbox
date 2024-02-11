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
		static void RaycastClosest(RaycastResult& result, Vec2f start, Vec2f end, Bitmask mask = 0);
		static void RaycastAll(std::vector<RaycastResult>& result, Vec2f start, Vec2f end, Bitmask mask = 0);
		static void CircleOverlap(std::vector<Body*>& result, Vec2f pos, float radius);
		static void PointTouch(std::vector<Body*>& result, Vec2f pos);
		static float BodyDistance(Body& lhs, Body& rhs);
	private:

		b2World* m_world;
		friend Singleton<Physics>;
	};
}