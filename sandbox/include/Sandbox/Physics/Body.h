#pragma once

#include "box2d/box2d.h"
#include "Sandbox/Vec.h"
#include "Sandbox/Physics/Bitmask.h"
#include "Sandbox/ECS/EntityId.h"
#include "Sandbox/std_macros.h"

namespace Sandbox
{
	class Collider;
	class PhysicsSystem;

	struct CollisionLayer
	{

	};
	/// @brief We can: 
	/// -add multiple colliders: Circle, Box, Polygons(triangle composition)
	/// -colliders rotation and position computed automatically by the transform component on same entity
	/// we can't:
	/// -remove collider
	/// -rescale collider
	/// -change collider shape
	/// -scale circle to make them ellipses
	/// atm: to remove or rescale collider you need to Destroy/Recreate body 
	struct Body
	{
	public:
		enum class Type
		{
			Static, Kinematic, Dynamic
		};

		Body(Type type, Bitmask layer = 1);
		~Body();
		/// @brief Set the layer it's on. 
		/// Warning: Must be set BEFORE adding colliders, (this will change in the future)
		/// @param layer 
		void SetLayer(Bitmask layer);
		/// @brief Set the layers it collide with. 
		/// Warning: Must be set BEFORE adding colliders, (this will change in the future)
		/// @param mask 
		void SetLayerMask(Bitmask mask);
		/// @brief Add a collider, 
		/// Warning: collider cannot be changed after being added. Make sure it's properly initialized
		/// (i.e : Polygon2D SetPoints before adding )
		/// @param collider 
		void AddCollider(sptr<Collider> collider);
		/// @brief Remove all the colliders, 
		/// Warning: not implemented. Destroy/Recreate the body instead
		void ClearCollider();

		bool BodyOverlap(Body* body);
		bool CircleOverlap(Vec2f point, float radius);
		bool PointInside(Vec2f point);
		void UpdateTransform(Vec3f position, float rotation);
		/// @brief Set true for collision to happen in the X/Z plane
		/// @param yIsZ 
		void SetYisZ(bool yIsZ);

		Bitmask GetLayer() const;
		Bitmask GetLayerMask() const;
		Type GetType() const;
		b2Body* GetB2Body();
		const std::vector<sptr<Collider>>* GetColliders();
		
	private:
		friend PhysicsSystem;
		b2Filter GetB2Filter();

		std::vector<sptr<Collider>> m_colliders;
		Type m_type;
		Bitmask m_layer;
		Bitmask m_mask;
		b2Body* m_body;
		EntityId m_entity;
		bool m_YisZ;
	};
}
