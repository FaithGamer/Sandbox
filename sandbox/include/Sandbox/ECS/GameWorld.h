#pragma once

#include "entt/entt.hpp"
#include "Sandbox/TypeId.h"
#include "Sandbox/Signal.h"


typedef entt::entity EntityId;

namespace Sandbox
{
	class Entity;
	class System;
	class Systems;

	struct ComponentSignal
	{
		Entity* entity;
	};
	/// @brief Contains all the entities, usually there is one game world.
	class GameWorld
	{
	public:
		/// @brief Create an entity
		/// @return the entity created.
		Entity& CreateEntity();

		/// @brief Destroy an entity
		/// @param entity The entity id to destroy
		void DestroyEntity(EntityId id);

		Entity* GetEntity(EntityId id);

		std::string GetName();
		
		/// @brief Bind a callback for when a component is added to an entity
		/// @param callback Method to call when the component is added
		/// @param listener Object to call the method upon
		/// @param priority Higher priority will receive message first.
		template <typename ComponentType, typename ListenerType> 	//To do: add free fuction
		void ListenOnAddComponent(
			void (ListenerType::* callback)(ComponentSignal), ListenerType* const listener, SignalPriority priority = SignalPriority::medium)
		{
			int typeId = TypeId::GetId<ComponentType>();

			auto findId = m_onAddComponent.find(typeId);
			if (findId == m_onAddComponent.end())
			{
				m_onAddComponent.insert(std::make_pair((int32_t)typeId, SignalSink(this)));
				m_registry.on_construct<ComponentType>().connect<&SignalSink::Send>(m_onAddComponent[typeId]);
			}
			m_onAddComponent[typeId].sender.AddListener(callback, listener, priority);
		}

		/// @brief Bind a callback for when a component is removed from an entity
		/// @param callback Method to call when the component is removed
		/// @param listener Object to call the method upon
		/// @param priority Higher priority will receive message first.
		template <typename ComponentType, typename ListenerType>	//To do: add free fuction
		void ListenOnRemoveComponent(
			void (ListenerType::* callback)(ComponentSignal), ListenerType* const listener, SignalPriority priority = SignalPriority::medium)
		{
			int typeId = TypeId::GetId<ComponentType>();

			auto findId = m_onRemoveComponent.find(typeId);
			if (findId == m_onRemoveComponent.end())
			{
				m_onRemoveComponent.insert(std::make_pair(typeId, SignalSink(this)));
				m_registry.on_construct<ComponentType>().connect<&SignalSink::Send>(m_onRemoveComponent[typeId]);
			}
			m_onRemoveComponent[typeId].sender.AddListener<ListenerType>(callback, listener, priority);
		}

		static GameWorld* GetMain();
		entt::registry m_registry;
	private:
		GameWorld(std::string name);
		friend Entity;
		friend System;
		friend Systems;
		struct SignalSink
		{
		public:
			SignalSink() : world(nullptr)
			{}
			SignalSink(GameWorld* w) : world(w)
			{}
			GameWorld* world;
			SignalSender<ComponentSignal> sender;
			void Send(entt::registry& registry, entt::entity enttId);
		};

		std::vector<Entity*> m_entities;

		std::unordered_map<int32_t, SignalSink> m_onAddComponent;
		std::unordered_map<int32_t, SignalSink> m_onRemoveComponent;

		std::string m_name;

		//To do: make theses variable changeable
		uint64_t m_entityPreallocationSize;
		uint32_t m_entityReallocationSize;
	};
}