#include "pch.h"

#include "Sandbox/ECS/GameWorld.h"
#include "Sandbox/ECS/Entity.h"
#include "Sandbox/Log.h"

namespace Sandbox
{
	GameWorld::GameWorld(std::string name) :
		m_name(name),
		m_entityPreallocationSize(10000),
		m_entityReallocationSize(100)
	{
		m_entities.reserve(m_entityPreallocationSize);
	}

	Entity& GameWorld::CreateEntity()
	{
		Entity* entity = new Entity(this);
		EntityId id = entity->m_id;

		//To do: free the reserved memory when capacity() is way over entt number
		if (m_entities.capacity() < (size_t)id)
		{
			m_entities.reserve(m_entities.capacity() + m_entityReallocationSize);
		}
		while (m_entities.size() < (size_t)id + 1)
		{
			m_entities.emplace_back(nullptr);
		}
		
		m_entities[(size_t)id] = entity;
		return *entity;
	}

	void GameWorld::DestroyEntity(EntityId id)
	{
		if (m_entities.size() <= (size_t)id || m_entities[(size_t)id] == nullptr)
		{
			LOG_WARN("Trying to destroy an entity that doesn't exists.");
		}
		else
		{
			delete m_entities[(size_t)id];
			m_entities[(size_t)id] = nullptr;
		}
	}

	Entity* GameWorld::GetEntity(EntityId id)
	{
		if (m_entities.size() <= (size_t)id || m_entities[(size_t)id] == nullptr)
		{
			LOG_WARN("Trying to get an entity that doesn't exists.");
			return nullptr;
		}
		else
		{
			return m_entities[(size_t)id];
		}
	}

	std::string GameWorld::GetName()
	{
		return m_name;
	}

	void GameWorld::SignalSink::Send(entt::registry& registry, entt::entity enttId)
	{
		sender.SendSignal(ComponentSignal(world->m_entities[(size_t)enttId]));
	}
}