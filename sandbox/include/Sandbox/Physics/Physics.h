#pragma once

#include "Sandbox/Singleton.h"
#include "Sandbox/Physics/AABBQueries.h"
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
		static void RaycastAll(std::vector<RaycastResult>& results, Vec2f start, Vec2f end, Bitmask mask = 65535);
		static void CircleOverlap(std::vector<OverlapResult>& results, Vec2f pos, float radius, Bitmask mask = 65535);
		static void PointInside(std::vector<OverlapResult>& results, Vec2f pos, Bitmask mask = 65535);
		static void BodyOverlap(std::vector<OverlapResult>& results, Body* body, Bitmask mask = 65535);
		//static float BodyDistance(Body& lhs, Body& rhs);

		/// @brief Add a collision layer with a custom name
		/// Generally call this method a bunch of time before launching the engine and creating Bodies
		/// @param layerName 
		static void AddLayer(String layerName)
		{
			Instance()->m_layers.AddFlag(layerName);
		}
		/// @brief Create a bitmask for the given collision layers. 
		/// Layers must have been added first using AddLayer.
		/// @return Bitmask of layers
		template <typename ...Str>
		static Bitmask GetLayerMask(Str... layers)
		{
			return Instance()->m_layers.GetMask(layers...);
		}

		/// @brief Enable/Disable drawing the colliders wireframe
		/// @param draw 
		static void DrawCollider(bool draw);
		static b2World* GetB2World();
	private:

		b2World* m_world;
		friend Singleton<Physics>;
		Filter m_layers;
	};
}