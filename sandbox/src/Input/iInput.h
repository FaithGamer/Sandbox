#pragma once

#include <SDL.h>
#include "Core/Log.h"
#include "entt/entt.hpp"
#include "Core/Signals.h"
#include "InputHelpers.h"
#include <nlohmann/json.hpp>


namespace Sandbox
{
	class InputMap;

	/// <summary>
	/// Holds one player input
	/// Keep track of the input state and dispatch events to listeners
	/// </summary>
	class iInput : public SignalSender, public std::enable_shared_from_this<iInput>
	{
	public:
		virtual ~iInput() {}

		virtual void KeyPressed(const SDL_Event& e) {}
		virtual void KeyReleased(const SDL_Event& e) {}
		virtual void MouseButtonPressed(const SDL_Event& e) {}
		virtual void MouseButtonReleased(const SDL_Event& e) {}
		virtual void MouseMoved(const SDL_Event& e) {}
		virtual void ControllerButtonPressed(const SDL_Event& e) {}
		virtual void ControllerButtonReleased(const SDL_Event& e) {}
		virtual void ControllerStickMoved(const SDL_Event& e) {}
		virtual void ControllerTriggerMoved(const SDL_Event& e) {}

		virtual void ListenEventAndBindTrigger(const SDL_Event& e, int version = 0) = 0;
		virtual void SetBindings(const Bindings& bindings, int version = 0) = 0;

		virtual std::string GetName() const = 0;
		virtual InputType GetType() const = 0;

	protected:
		
		virtual void UpdateEventListened() = 0;
		void NotifyUpdateEvent();
		InputEvent m_eventsListened;

	private:

		friend class InputMap;
		InputMap* m_inputMap = nullptr;	
	};
}