#pragma once
#include "Sandbox/std_macros.h"
#include "Sandbox/Time.h"
#include <SDL/SDL.h>

namespace Sandbox
{


	class System
	{
	public:

		typedef enum
		{
			Update = 1,
			FixedUpdate = 2,
			Event = 3,
			ImGui = 4 //To do: add OnRender ?
		}Method;

		virtual ~System() {}

		/// @brief Called when the system is pushed within Systems
		virtual void OnStart() {}

		/// @brief Called as often as possible.
		/// @param time Time elapsed since last call.
		virtual void OnUpdate(Time time) {}

		/// @brief Called on a fixed timestep.
		/// The timestep can be accessed through Systems::GetFixedTimeStep()
		virtual void OnFixedUpdate() {}

		/// @brief This is where you can create ImGui elements
		virtual void OnImGui() {}

		/// @brief Called every time an SDL_Event is received, if no other system has processed the event.
		/// @param event SDL_Event
		/// @return true if the event has been
		virtual bool OnEvent(SDL_Event& event) { return false; }

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
		virtual int GetUsedMethod() { return Update | FixedUpdate | Event | ImGui; }

		/// @brief Higher priority will have it's methods called before lower priority
		virtual int GetPriority() { return m_priority; }

		/// @brief Gives an string identifier to the system, for debugging purposes
		virtual std::string DebugName() { return "System"; }

	protected:

	private:

		int m_priority = 0;

	};
}