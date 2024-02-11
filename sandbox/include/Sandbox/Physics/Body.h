#pragma once

#include "box2d/box2d.h"

namespace Sandbox
{
	class Collider;

	struct CollisionLayer
	{

	};
	struct Body
	{
	public:
		enum class Type
		{
			Static, Kinematic, Dynamic
		};

		Body(Type type);
		/// @brief Set the layer it's on
		/// @param layer 
		void SetLayer(Bitmask layer);
		/// @brief Set the layers it collide with
		/// @param mask 
		void SetLayerMask(Bitmask mask);

		void AddCollider(sptr<Collider> collider);

		bool BodyOverlap(Body* body);
		bool CircleOverlap(Vec2f point, float radius);
		bool PointInside(Vec2f point);

		
	private:
		void CreateStatic();
		void CreateKinematic();
		void CreateDynamic();


		b2Body m_body;
	};
}
