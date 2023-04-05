#pragma once

#include "entt/entt.hpp"
#include "TypeId.h"
#include "Sandbox/Signal.h"

namespace Sandbox
{
	class Entity;

	struct ComponentSignal
	{
		sptr<Entity> entity;
	};


	/// @brief usually there is one game world.
	class GameWorld
	{
	public:
		GameWorld();
		
		sptr<Entity> AddEntity();
		void RemoveEntity(sptr<Entity> entity);

		//To do: add free fuction
		template <typename ComponentType, typename ListenerType>
		void ListenOnAddComponent(
			void (ListenerType::* callback)(ComponentSignal), ListenerType* const listener, SignalPriority priority = SignalPriority::medium)
		{
			int typeId = TypeId::GetId<T>();

			auto findId = m_onAddComponent.find(typeId);
			if (findId == m_onAddComponent.end())
			{
				m_onAddComponent.insert(std::make_pair(typeId, SignalSink(this)));
				m_registry.on_construct<T>().connect<&SignalSink::Send>(m_onAddComponent[typeId]);
			}
			m_onAddComponent[typeId].AddListener(callback, listener, priority);
		}

		//To do: add free fuction
		template <typename ComponentType, typename ListenerType>
		void ListenOnRemoveComponent(
			void (ListenerType::* callback)(ComponentSignal), ListenerType* const listener, SignalPriority priority = SignalPriority::medium)
		{
			int typeId = TypeId::GetId<T>();

			auto findId = m_onRemoveComponent.find(typeId);
			if (findId == m_onRemoveComponent.end())
			{
				m_onRemoveComponent.insert(std::make_pair(typeId, SignalSink(this)));
				m_registry.on_construct<T>().connect<&SignalSink::Send>(m_onRemoveComponent[typeId]);
			}
			m_onRemoveComponent[typeId].AddListener(callback, listener, priority);
		}

		entt::registry m_registry;
	private:

		struct SignalSink
		{
		public:
			SignalSink(GameWorld* w) : world(w)
			{}
			GameWorld* world;
			SignalSender<ComponentSignal> m_sender;
			void Send(entt::registry& registry, entt::entity enttId);
		};

		std::vector<sptr<Entity>> m_enttIdToEntity;

		std::unordered_map<int32_t, SignalSink> m_onAddComponent;
		std::unordered_map<int32_t, SignalSink> m_onRemoveComponent;

		//To do: make theses variable changeable
		uint64_t m_entityPreallocationSize;
		uint32_t m_entityReallocationSize;
	};
}