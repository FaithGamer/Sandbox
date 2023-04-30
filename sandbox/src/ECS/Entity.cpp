#include "pch.h"
#include "Sandbox/ECS/Entity.h"

namespace Sandbox
{
	Entity::Entity() : m_free(true)
	{

	}

	Entity::Entity(entt::registry* registry, EntityId id) : m_registry(registry), m_id(id), m_free(false)
	{

	}
	
	Entity::~Entity()
	{

	}

	void Entity::Free()
	{
		m_registry->destroy(m_id);
		m_free = true;
	}

	bool Entity::IsFree() const
	{
		return m_free;
	}

	EntityId Entity::GetId() const
	{
		return m_id;
	}
}