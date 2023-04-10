#pragma once
#include "Sandbox/std_macros.h"
#include "entt/entt.hpp"
#include "Sandbox/ECS/GameWorld.h"

namespace Sandbox
{
	class Entity
	{
	public:
		~Entity();

		//To do: copy constructo and operator=,
		//to really copy the component, have typeid vector of contained components.
		
		/// @brief Add a component if it doesn't exists yet.
		/// @param args Parameters for the component constructor.
		/// @return Added component, or the one already in place.
		template <typename ComponentType, typename... Args>
		ComponentType& AddComponent(Args&&... args)
		{
			return m_world->m_registry.get_or_emplace<ComponentType>(m_id, std::forward<Args>(args)...);
		}
		/// @brief Remove a component
		/// @tparam ComponentType 
		template <typename ComponentType>
		void RemoveComponent()
		{
			m_world->m_registry.remove<ComponentType>(m_id);
		}
		/// @brief Access an entity component if it exists.
		/// Do not store the pointer as it may be invalidated.
		/// @return Component pointer, nullptr if doesn't exists.
		template <typename ComponentType>
		ComponentType* GetComponent()
		{
			return m_world->m_registry.try_get<ComponentType>(m_id);
		}
		/// @brief Access an entity component if it exists
		/// @return Component reference, nullptr if doesn't exists.
		template <typename ComponentType>
		ComponentType& GetComponentNoCheck()
		{
			return m_world->m_registry.get<ComponentType>(m_id);
		}
		/// @brief Get the EntityId
		/// The EntityId will remain the same during the entity lifetime.
		/// It can be used to retreive the entity from it's GameWorld with no overhead.
		/// @return The EntityId
		EntityId GetId() const;
	private:
		friend GameWorld;
		Entity(GameWorld* world);
		
		EntityId m_id;
		GameWorld* m_world;
	};
}