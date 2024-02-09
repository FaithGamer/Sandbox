#include "pch.h"

#include "Sandbox/ECS/World.h"
#include "Sandbox/Log.h"


namespace Sandbox
{
	World::World(std::string name) :
		m_name(name)
	{
	}

	bool World::HaveEntity(EntityId entity)
	{
		return registry.valid(entity);
	}

	bool World::HaveEntity(Entity entity)
	{
		return registry.valid(entity.GetId());
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