#include "pch.h"

#include "Sandbox/ECS/World.h"
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

	Entity World::CreateEntity()
	{
		EntityId id = registry.create();
		return Entity(&registry, id);
	}

	void World::DestroyEntity(EntityId id)
	{
		registry.destroy(id);
	}

	Entity World::GetEntity(EntityId id)
	{
		return Entity(&registry, id);
	}

	std::string World::GetName()
	{
		return m_name;
	}

	void World::SignalSink::Send(entt::registry& registry, entt::entity enttId)
	{
		sender.SendSignal(ComponentSignal(enttId));
	}
}