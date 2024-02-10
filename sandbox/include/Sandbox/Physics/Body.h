#pragma once

#include "box2d/box2d.h"

namespace Sandbox
{
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
		void SetLayerFlag(uint16 layer);
		/// @brief Set the layers it collide with
		/// @param mask 
		void SetLayerMask(uint16 mask);
	private:
		void CreateStatic();
		void CreateKinematic();
		void CreateDynamic();


		b2Body m_body;
		b2World* m_world;
	};
}
