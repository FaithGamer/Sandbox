#pragma once

#include <SDL/SDL.h>
#include <entt/entt.hpp>

#include "Sandbox/Log.h"
#include "Sandbox/Signal.h"
#include "Sandbox/Vec.h"

namespace Sandbox
{
	class InputMap;


	/// @brief Type of Input, each type may hold a different state or send different type of signal
	enum class InputType : int
	{
		Button = 0,
		Pointing,
		Directional,
		Scroll,
		Textual
	};

	/// @brief What events does an input listen to
	struct InputEvent
	{
		bool keyButton = false;
		bool keyText = false;
		bool mouseButton = false;
		bool mouseMovement = false;
		bool controllerButton = false;
		bool controllerStick = false;
		bool controllerTrigger = false;

		bool operator==(const InputEvent& other);
		bool operator!=(const InputEvent& other);
	};

	/// @brief Parent class for data sent by input when they are triggered.
	class InputSignal
	{
	public:
		virtual bool GetBool() const 
		{
			LOG_WARN("Input Signal GetBool not implemented, false returned.");
			return false;
		}
		virtual Vec2f GetVec2f() const 
		{ 
			LOG_WARN("Input Signal GetVec2f not implemented, default value returned.");
			return Vec2f();
		}
		virtual char* GetText() const 
		{ 
			LOG_WARN("Input Signal GetText not implemented, nullptr returned.");
			return nullptr; 
		}
		virtual float GetFloat() const
		{
			LOG_WARN("Input Signal GetFloat not implemented, 0.0f returned.");
			return 0.0f;
		}
	};

	/// @brief The name of an InputType
	std::string InputTypeName(InputType type);

	/// @brief Interface for an input, send signal when it's binding is triggered
	class Input : public std::enable_shared_from_this<Input>
	{
	public:
		virtual ~Input() {};

		virtual void ListenEventAndBind(const SDL_Event& e, int version = 0) = 0;

		virtual std::string GetName() const = 0;
		virtual InputType GetType() const = 0;

		SignalSender<InputSignal*> signal;

		//To do, add enable mouse and keyboard/controller

	protected:
		Input() {};
		friend class InputMap;

		virtual bool KeyPressed(const SDL_Event& e) { return false; }
		virtual bool KeyReleased(const SDL_Event& e) { return false; }
		virtual bool MouseButtonPressed(const SDL_Event& e) { return false; }
		virtual bool MouseButtonReleased(const SDL_Event& e) { return false; }
		virtual bool MouseMoved(const SDL_Event& e) { return false; }
		virtual bool ControllerButtonPressed(const SDL_Event& e) { return false; }
		virtual bool ControllerButtonReleased(const SDL_Event& e) { return false; }
		virtual bool ControllerStickMoved(const SDL_Event& e) { return false; }
		virtual bool ControllerTriggerMoved(const SDL_Event& e) { return false; }
		virtual bool TextEntered(const SDL_Event& e) { return false; }

		virtual void UpdateEventListened() = 0;
		void OnEventListenedUpdated();
		InputEvent m_eventsListened;

	private:

		friend class InputMap;
		InputMap* m_inputMap = nullptr;
	};
}