#include "pch.h"
#include "Sandbox/ECS/Entity.h"
#include "Sandbox/ECS/World.h"
#include "Sandbox/ECS/Systems.h"
namespace Sandbox
{
	Entity::Entity()
	{
		auto world = Systems::GetMainWorld();
		m_id = world->registry.create();
		m_registry = &world->registry;
		
	}
	Entity::Entity(World* world)
	{
		m_id = world->registry.create();
		m_registry = &world->registry;
	}
	Entity::Entity(entt::registry* registry, EntityId id) : m_registry(registry), m_id(id)
	{

	}
	void Entity::Destroy() const
	{
		m_registry->destroy(m_id);
	}
}