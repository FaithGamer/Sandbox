#include "pch.h"

#include "Sandbox/ECS/World.h"
#include "Sandbox/ECS/Systems.h"
#include "Sandbox/Log.h"


namespace Sandbox
{
	World::World(std::string name) :
		m_name(name),
		m_entityPreallocationSize(10000),
		m_entityReallocationSize(100)
	{
		m_entities.reserve(m_entityPreallocationSize);
	}

	Entity* World::CreateEntity()
	{
		EntityId id = m_registry.create();
		
		//To do: free the reserved memory when capacity() is way over entt number

		if (m_entities.capacity() < (size_t)id)
		{
			m_entities.reserve(m_entities.capacity() + m_entityReallocationSize);
		}
		while (m_entities.size() < (size_t)id + 1)
		{
			m_entities.emplace_back(Entity());
		}
		m_entities[(size_t)id] = Entity(&m_registry, id);
		auto i = m_entities[(size_t)id].GetId();
		return &m_entities[(size_t)id];
	}

	void World::DestroyEntity(EntityId id)
	{
		if (m_entities.size() <= (size_t)id || m_entities[(size_t)id].IsFree())
		{
			LOG_WARN("Trying to destroy an entity that doesn't exists.");
		}
		else
		{
			m_entities[(size_t)id].Free();
		}
	}

	Entity* World::GetEntity(EntityId id)
	{
		if (m_entities.size() <= (size_t)id || m_entities[(size_t)id].IsFree())
		{
			LOG_WARN("Trying to get an entity that doesn't exists.");
			return nullptr;
		}
		else
		{
			return &m_entities[(size_t)id];
		}
	}

	std::string World::GetName()
	{
		return m_name;
	}

	void World::SignalSink::Send(entt::registry& registry, entt::entity enttId)
	{
		sender.SendSignal(ComponentSignal(&world->m_entities[(size_t)enttId]));
	}

	World* World::GetMain()
	{
		return Systems::GetMainWorld();
	}
}