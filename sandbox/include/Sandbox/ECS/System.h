#pragma once
#include "Sandbox/std_macros.h"
#include "Sandbox/Time.h"
#include <SDL/SDL.h>
#include "Sandbox/ECS/World.h"

namespace Sandbox
{
	class Entity;

	class System
	{
	public:

		typedef enum
		{
			Update = 1,
			FixedUpdate = 2,
			Event = 4,
			ImGui = 8,
			Render = 16
		}Method;

		virtual ~System() {}

		/// @brief Called when the system is pushed within Systems
		virtual void OnStart() {}

		/// @brief Called as often as possible.
		/// @param deltaTime Time elapsed since last call.
		virtual void OnUpdate(Time deltaTime) {}

		/// @brief Called on a fixed timestep.
		virtual void OnFixedUpdate(Time fixedDeltaTime) {}

		/// @brief Make all your rendering here. Called in between Window::Clear and Window::Render
		virtual void OnRender() {}

		/// @brief This is where you can create ImGui elements
		virtual void OnImGui() {}

		/// @brief Called every time an SDL_Event is received, if no other system has processed the event.
		/// @param event SDL_Event
		/// @return true if the event has been
		virtual bool OnEvent(SDL_Event& event, bool handledByImGui) { return false; }

		/// @brief Called when the system is removed from Systems
		virtual void OnRemove() {}

		/// @brief Use this method in the system's constructor to override default value.
		/// @param priority If set to 0, Systems will attribute an automatic value based on the order it has been pushed.
		void SetPriority(int priority)
		{
			m_priority = priority;
		}

		/// @brief bitmask telling Sandbox wich method are being used.
		/// overriding this will help the engine save some amount of CPU power.
		virtual int GetUsedMethod() { return Update | FixedUpdate | Event | ImGui | Render; }

		/// @brief Higher priority will have it's methods called before lower priority
		virtual int GetPriority() { return m_priority; }

		/// @brief Gives an string identifier to the system, for debugging purposes
		virtual std::string DebugName() { return "System"; }

	protected:
		std::vector<World*>& GetWorlds();
		//To do: documentation

		template <typename... ComponentType, typename Functor>
		void ForEachComponent(Functor function)
		{
			for (auto& world : GetWorlds())
			{
				auto view = world->m_registry.view<ComponentType...>();
				for (auto entityId : view)
				{
					[&] <std::size_t... I>(std::index_sequence<I...>)
					{
						function(std::get<I>(view.get(entityId))...);
					}(std::make_index_sequence<sizeof...(ComponentType)>());
				}
			}
		};

		template <typename... ComponentType, typename Functor>
		void ForEachEntity(Functor function)
		{
			for (auto& world : GetWorlds())
			{
				auto view = world->m_registry.view<ComponentType...>();
				for (auto entityId : view)
				{
					[&] <std::size_t... I>(std::index_sequence<I...>)
					{
						Entity* entity = world->GetEntity(entityId);
						function(*entity, std::get<I>(view.get(entityId))...);
					}(std::make_index_sequence<sizeof...(ComponentType)>());
				}
			}
		};

		template <typename... ComponentType, typename Functor>
		void ForEachComponent(World* world, Functor function)
		{
			auto view = world->m_registry.view<ComponentType...>();
			for (auto entityId : view)
			{
				[&] <std::size_t... I>(std::index_sequence<I...>)
				{
					function(std::get<I>(view.get(entityId))...);
				}(std::make_index_sequence<sizeof...(ComponentType)>());
			}
		};

		template <typename... ComponentType, typename Functor>
		void ForEachEntity(World* world, Functor function)
		{
			auto view = world->m_registry.view<ComponentType...>();
			for (auto entityId : view)
			{
				[&] <std::size_t... I>(std::index_sequence<I...>)
				{
					Entity* entity = world->GetEntity(entityId);
					function(*entity, std::get<I>(view.get(entityId))...);
				}(std::make_index_sequence<sizeof...(ComponentType)>());
			}
		};

		template <typename... ComponentType, typename SystemType>
		void ForEachComponent(void(SystemType::* function)(ComponentType&...))
		{
			for (auto& world : GetWorlds())
			{
				auto view = world->m_registry.view<ComponentType...>();
				for (auto entityId : view)
				{
					[&] <std::size_t... I>(std::index_sequence<I...>)
					{
						(static_cast<SystemType*>(this)->*function)(std::get<I>(view.get(entityId))...);
					}(std::make_index_sequence<sizeof...(ComponentType)>());
				}
			}
		};

		template <typename... ComponentType, typename SystemType>
		void ForEachEntity(void(SystemType::* function)(Entity&, ComponentType&...))
		{
			World* world = World::GetMain();
			auto view = world->m_registry.view<ComponentType...>();
			for (auto entityId : view)
			{
				[&] <std::size_t... I>(std::index_sequence<I...>)
				{
					Entity* entity = world->GetEntity(entityId);
					(static_cast<SystemType*>(this)->*function)(*entity, std::get<I>(view.get(entityId))...);
				}(std::make_index_sequence<sizeof...(ComponentType)>());
			}
		};

		template <typename... ComponentType, typename SystemType>
		void ForEachComponent(World* world, void(SystemType::* function)(ComponentType&...))
		{
			auto view = world->m_registry.view<ComponentType...>();
			for (auto entityId : view)
			{
				[&] <std::size_t... I>(std::index_sequence<I...>)
				{
					(static_cast<SystemType*>(this)->*function)(std::get<I>(view.get(entityId))...);
				}(std::make_index_sequence<sizeof...(ComponentType)>());
			}
		};

		template <typename... ComponentType, typename SystemType>
		void ForEachEntity(World* world, void(SystemType::* function)(Entity&, ComponentType&...))
		{
			auto view = world->m_registry.view<ComponentType...>();
			for (auto entityId : view)
			{
				[&] <std::size_t... I>(std::index_sequence<I...>)
				{
					Entity* entity = world->GetEntity(entityId);
					(static_cast<SystemType*>(this)->*function)(*entity, std::get<I>(view.get(entityId))...);
				}(std::make_index_sequence<sizeof...(ComponentType)>());
			}
		};

		template <typename... ComponentType, typename SystemType>
		void ForEachComponent(void(SystemType::* function)(Time delta, ComponentType&...), Time delta)
		{
			for (auto& world : GetWorlds())
			{
				auto view = world->m_registry.view<ComponentType...>();
				for (auto entityId : view)
				{
					[&] <std::size_t... I>(std::index_sequence<I...>)
					{
						(static_cast<SystemType*>(this)->*function)(delta, std::get<I>(view.get(entityId))...);
					}(std::make_index_sequence<sizeof...(ComponentType)>());
				}
			}
		};

		template <typename... ComponentType, typename SystemType>
		void ForEachEntity(void(SystemType::* function)(Time delta, Entity& entity, ComponentType&...), Time delta)
		{
			for (auto& world : GetWorlds())
			{
				auto view = world->m_registry.view<ComponentType...>();
				for (auto entityId : view)
				{
					[&] <std::size_t... I>(std::index_sequence<I...>)
					{
						(static_cast<SystemType*>(this)->*function)(delta, std::get<I>(view.get(entityId))...);
					}(std::make_index_sequence<sizeof...(ComponentType)>());
				}
			}
		};


	private:

		int m_priority = 0;

	};
}