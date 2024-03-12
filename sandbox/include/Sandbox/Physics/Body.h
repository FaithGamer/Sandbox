#pragma once

#include "box2d/box2d.h"
#include "Sandbox/Core/Vec.h"
#include "Sandbox/Physics/Collider.h"
#include "Sandbox/Physics/Bitmask.h"
#include "Sandbox/ECS/EntityId.h"
#include "Sandbox/Core/std_macros.h"

namespace Sandbox
{
	class Collider;
	class PhysicsSystem;

	struct RaycastResult
	{
		EntityId entityId = EntityId(0);
		Vec2f point = 0;
		Vec2f normal = 0;
		float distance = 0;
		bool hit = false;

		operator bool()
		{
			return hit;
		}
	};

	struct OverlapResult
	{
		EntityId entityId;
		float distance;
	};

	struct CollisionLayer
	{

	};
	/// @brief We can: 
	/// -add multiple colliders: Circle, Box, Polygons (not implemented yet)
	/// -colliders rotation and position computed automatically by the PhysicsSystem from the transform component on same entity
	/// we can't:
	/// -remove collider
	/// -rescale collider
	/// -change collider shape
	/// -scale circle to make them ellipses
	struct Body
	{
	public:

		/// @brief There's no difference between different types at the moment
		/// Ultimately the goal is to have Kinematic and Dynamic be able to interpolate between two fixed step
		/// Dynamic would automatically collide and react to  physic.
		/// I don't even know yet what should be the difference between kinematic and static tho.
		/// Also different body type might better be different classes
		enum class Type
		{
			Static, Kinematic, Dynamic
		};

		Body(Type type = Type::Kinematic, Bitmask layer = 1);
		Body(Body&& body) noexcept;
		~Body();
		/// @brief Set the layers it's on. 
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

		/// @brief Get the list of overlapping bodies within the approriate collision layer
		/// @param results 
		void OverlappingBodies(std::vector<OverlapResult>& results);
		/// @brief Not implemeneted
		bool BodyOverlap(Body* body);
		/// @brief Not implemeneted
		bool CircleOverlap(Vec2f point, float radius);
		/// @brief Not implemeneted
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
		b2AABB GetAABB();

	public:
		/// @brief Used internally to keep data about the body within box2D


		Collider::UserData userData;

	private:


		friend PhysicsSystem;
		b2Filter GetB2Filter();

		std::vector<sptr<Collider>> m_colliders;
		Type m_type;
		Bitmask m_layer;
		Bitmask m_mask;
		b2Body* m_b2Body;

		bool m_YisZ;
	};
}
