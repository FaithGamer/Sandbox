#include "pch.h"
#include "Sandbox/ECS/Entity.h"
#include "Sandbox/ECS/World.h"
#include "Sandbox/ECS/Systems.h"
namespace Sandbox
{
	Entity::Entity() : m_valid(false), m_registry(nullptr)
	{
	
		
	}

	Entity Entity::Create()
	{
		Entity entity;
		auto world = Systems::GetMainWorld();
		entity.m_id = world->registry.create();
		entity.m_registry = &world->registry;
		entity.m_valid = true;
		return entity;
	}

	Entity Entity::Create(World* world)
	{
		Entity entity;
		entity.m_id = world->registry.create();
		entity.m_registry = &world->registry;
		entity.m_valid = true;
		return entity;
	}


	Entity::Entity(EntityId entityId) : m_id(entityId)
	{
		m_registry = &Systems::GetMainWorld()->registry;
		m_valid = true;
	}

	Entity::Entity(EntityId entityId, World* world) : m_id(entityId)
	{
		m_registry = &Systems::GetMainWorld()->registry;
		m_valid = true;
	}

	bool Entity::Valid()
	{
		return m_valid && m_registry->valid(m_id);
	}

	void Entity::Destroy()
	{
		m_valid = false;
		m_registry->destroy(m_id);
	}
}