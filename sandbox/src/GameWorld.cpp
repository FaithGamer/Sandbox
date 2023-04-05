#include "pch.h"

#include "Sandbox/GameWorld.h"
#include "Sandbox/Entity.h"

namespace Sandbox
{
	GameWorld::GameWorld() :
		m_entityPreallocationSize(10000),
		m_entityReallocationSize(100)
	{
		m_enttIdToEntity.reserve(m_entityPreallocationSize);
	}

	sptr<Entity> GameWorld::AddEntity()
	{
		auto entity = makesptr<Entity>(this);
		entt::entity enttId = entity->m_enttId;
		while (m_enttIdToEntity.capacity() < (size_t)enttId)
		{
			m_enttIdToEntity.reserve(m_enttIdToEntity.capacity() + m_entityReallocationSize);
		}
		//To do: clear the unsued value when size() > entt number
		m_enttIdToEntity[(size_t)enttId] = entity;
		return entity;
	}

	void GameWorld::RemoveEntity(sptr<Entity> entity)
	{
		m_registry.destroy(entity->m_enttId);
		m_enttIdToEntity[(size_t)entity->m_enttId].reset();
	}

	void GameWorld::SignalSink::Send(entt::registry& registry, entt::entity enttId)
	{
		sptr<Entity> entity = world->m_enttIdToEntity[(size_t)enttId];
		m_sender.SendSignal(ComponentSignal(entity));
	}
}