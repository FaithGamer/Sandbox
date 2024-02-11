#include "pch.h"
#include "Sandbox/ECS/Entity.h"
#include "Sandbox/ECS/World.h"
#include "Sandbox/ECS/Systems.h"
#include "Sandbox/Render/Transform.h"

namespace Sandbox
{
	Entity::Entity() : m_valid(false), m_registry(nullptr)
	{


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

	void Entity::AddChild(Entity entity)
	{
		if (!entity.Valid())
		{
			LOG_ERROR("Cannot add an invalid entity as a children.");
			return;
		}
		if (entity.m_registry != m_registry)
		{
			LOG_ERROR("Cannot add an entity child if it doesn't belong to the same world as parent.");
			return;
		}

		entity.JustUnparent();

		auto children = AddComponent<Children>();
		children->children.insert(entity.m_id);

		auto transformChildren = entity.GetComponent<Transform>();
		transformChildren->SetParent(m_id);

	}

	void Entity::RemoveChild(EntityId entity)
	{
		auto children = GetComponent<Children>();

		if (children == nullptr)
		{
			LOG_WARN("Trying to remove child on an entity with no children. What are you doing ?");
			return;
		}

		auto find_it = children->children.find(entity);
		if (find_it == children->children.end())
		{
			LOG_WARN("Trying to remove a child entity that is not in the parent. You looser.");
			return;
		}

		Entity(*find_it, m_registry).JustUnparent();

		if (children->children.empty())
		{
			RemoveComponent<Children>();
		}
	}

	void Entity::Unparent()
	{
		auto transform = GetComponent<Transform>();
		if (transform != nullptr)
		{
			transform->RemoveParent();
			return;
		}
		auto parent = GetComponent<Parent>();
		if (parent != nullptr)
		{
			Entity(parent->parent).JustRemoveChild(m_id);
		}
		RemoveComponent<Parent>();

	}

	bool Entity::Valid()
	{
		return m_valid && m_registry->valid(m_id);
	}

	void Entity::Destroy()
	{
		auto children = GetComponent<Children>();
		if (children != nullptr)
		{
			for (auto& child : children->children)
			{
				Entity(child, m_registry).Destroy();
			}
		}
		m_valid = false;
		m_registry->destroy(m_id);
	}

	void Entity::JustUnparent()
	{
		auto transform = GetComponent<Transform>();
		if (transform != nullptr)
		{
			transform->RemoveParent();
			return;
		}
		RemoveComponent<Parent>();
	}

	void Entity::JustRemoveChild(EntityId id)
	{
		auto children = GetComponent<Children>();
		if (children == nullptr)
			return;

		children->children.erase(id);
		if (children->children.empty())
		{
			RemoveComponent<Children>();
		}
	}

	Entity::Entity(EntityId entityId, entt::registry* registry) : m_id(entityId), m_registry(registry), m_valid(true)
	{

	}
}