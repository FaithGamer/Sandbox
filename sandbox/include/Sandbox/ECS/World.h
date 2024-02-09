#pragma once

#include "entt/entt.hpp"
#include "Sandbox/TypeId.h"
#include "Sandbox/Signal.h"
#include "Sandbox/ECS/Entity.h"

namespace Sandbox
{

	class System;
	class Systems;

	struct ComponentSignal
	{
		EntityId entity;
	};
	/// @brief Contains all the entities, usually there is one game world.
	class World
	{
	public:

		/// @brief Does this world contains this entity (given the entity is valid)
		bool HaveEntity(Entity entity);
		/// @brief Does this world contains this entity (given the entity is valid)
		bool HaveEntity(EntityId entity);
		/// @brief Get the count of entity 
		inline unsigned int GetEntityCount()
		{
			return (unsigned int)registry.alive();
		}

		std::string GetName();
		
		/// @brief Bind a callback for when a component is added to an entity
		/// @param callback Method to call when the component is added
		/// @param listener Object to call the method upon
		/// @param priority Higher priority will receive message first.
		template <typename ComponentType, typename ListenerType>
		void ListenAddComponentSignal(
			void (ListenerType::* callback)(ComponentSignal), ListenerType* const listener, SignalPriority priority = SignalPriority::medium)
		{
			int typeId = TypeId::GetId<ComponentType>();

			auto findId = m_onAddComponent.find(typeId);
			if (findId == m_onAddComponent.end())
			{
				m_onAddComponent.insert(std::make_pair((int32_t)typeId, SignalSink(this)));
				registry.on_construct<ComponentType>().connect<&SignalSink::Send>(m_onAddComponent[typeId]);
			}
			m_onAddComponent[typeId].sender.AddListener(callback, listener, priority);
		}

		/// @brief Bind a callback for when a component is removed from an entity
		/// @param callback Method to call when the component is removed
		/// @param listener Object to call the method upon
		/// @param priority Higher priority will receive message first.
		template <typename ComponentType, typename ListenerType>
		void ListenRemoveComponentSignal(
			void (ListenerType::* callback)(ComponentSignal), ListenerType* const listener, SignalPriority priority = SignalPriority::medium)
		{
			int typeId = TypeId::GetId<ComponentType>();

			auto findId = m_onRemoveComponent.find(typeId);
			if (findId == m_onRemoveComponent.end())
			{
				m_onRemoveComponent.insert(std::make_pair(typeId, SignalSink(this)));
				registry.on_construct<ComponentType>().connect<&SignalSink::Send>(m_onRemoveComponent[typeId]);
			}
			m_onRemoveComponent[typeId].sender.AddListener<ListenerType>(callback, listener, priority);
		}

		template <typename ComponentType, typename ListenerType>
		void StopListenAddComponentSignal(ListenerType* const listener)
		{
			int typeId = TypeId::GetId<ComponentType>();

			auto findId = m_onAddComponent.find(typeId);
			if (findId == m_onAddComponent.end())
			{
				m_onAddComponent.insert(std::make_pair((int32_t)typeId, SignalSink(this)));
				registry.on_construct<ComponentType>().connect<&SignalSink::Send>(m_onAddComponent[typeId]);
			}
			m_onAddComponent[typeId].sender.RemoveListener(listener);
		}

		template <typename ComponentType, typename ListenerType>
		void StopListenRemoveComponentSignal(ListenerType* const listener)
		{
			int typeId = TypeId::GetId<ComponentType>();

			auto findId = m_onRemoveComponent.find(typeId);
			if (findId == m_onRemoveComponent.end())
			{
				m_onRemoveComponent.insert(std::make_pair(typeId, SignalSink(this)));
				registry.on_construct<ComponentType>().connect<&SignalSink::Send>(m_onRemoveComponent[typeId]);
			}
			m_onRemoveComponent[typeId].sender.RemoveListener(listener);
		}

		entt::registry registry;
	private:
		World(std::string name);
		friend Entity;
		friend System;
		friend Systems;
		struct SignalSink
		{
		public:
			SignalSink() : world(nullptr)
			{}
			SignalSink(World* w) : world(w)
			{}
			World* world;
			SignalSender<ComponentSignal> sender;
			void Send(entt::registry& registry, entt::entity enttId);
		};

		std::unordered_map<int32_t, SignalSink> m_onAddComponent;
		std::unordered_map<int32_t, SignalSink> m_onRemoveComponent;

		std::string m_name;

	};
}