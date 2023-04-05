#pragma once
#include "Sandbox/std_macros.h"
#include "entt/entt.hpp"
#include "Sandbox/GameWorld.h"

typedef uint64_t UID;
class GameWorld;

namespace Sandbox
{
	class Entity
	{
	public:
		
		template <typename T>
		T& AddComponent()
		{
			return m_world->m_registry.emplace<T>();
		}
		template <typename T>
		void RemoveComponent()
		{
			m_world->m_registry.remove<T>(m_enttId);
		}
		template <typename T>
		T* GetComponent()
		{
			return m_world->m_registry.try_get<T>();
		}
		template <typename T>
		T& GetComponentUnsafe()
		{
			return m_world->m_registry.get<T>();
		}

		//entt::entity GetEnttId() const;

		UID GetUID();
	private:
		friend GameWorld;
		Entity(GameWorld* world);
		~Entity();

		entt::entity m_enttId;
		GameWorld* m_world;
	};
}