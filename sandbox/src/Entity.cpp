#include "pch.h"
#include "Sandbox/Entity.h"

namespace Sandbox
{
	Entity::Entity(GameWorld* world)
	{
		m_world = world;
		m_enttId = world->m_registry.create();
	}
	Entity::~Entity()
	{
		m_world->m_registry.release(m_enttId);
	}
	/*entt::entity GetEnttId() const
	{

	}*/
}