#include "pch.h"
#include "Sandbox/ECS/Entity.h"
#include "Sandbox/ECS/GameWorld.h"

namespace Sandbox
{
	Entity::Entity(GameWorld* world)
	{
		m_id = world->m_registry.create();
		m_world = world;
	}
	Entity::~Entity()
	{
		m_world->m_registry.destroy(m_id);
	}
	EntityId Entity::GetId() const
	{
		return m_id;
	}
}