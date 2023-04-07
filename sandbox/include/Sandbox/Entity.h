#pragma once
#include "Sandbox/std_macros.h"
#include "entt/entt.hpp"
#include "Sandbox/GameWorld.h"

namespace Sandbox
{
	class Entity
	{
	public:
		~Entity();
		
		template <typename ComponentType>
		ComponentType& AddComponent()
		{
			return m_world->m_registry.emplace<ComponentType>(m_id);
		}
		template <typename ComponentType>
		void RemoveComponent()
		{
			m_world->m_registry.remove<ComponentType>(m_id);
		}
		template <typename ComponentType>
		ComponentType* GetComponent()
		{
			return m_world->m_registry.try_get<ComponentType>(m_id);
		}
		template <typename ComponentType>
		ComponentType& GetComponentUnsafe()
		{
			return m_world->m_registry.get<ComponentType>(m_id);
		}

		EntityId GetId() const;
	private:
		friend GameWorld;
		Entity(GameWorld* world);
		

		EntityId m_id;
		GameWorld* m_world;
	};
}