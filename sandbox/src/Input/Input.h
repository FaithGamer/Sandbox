#pragma once

#include <SDL.h>
#include "Core/Log.h"
#include "entt/entt.hpp"
#include "Core/Signal.h"
#include <nlohmann/json.hpp>


namespace Sandbox
{
	class InputMap;
	class Bindings;
	enum class DirectionalButton;

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

	struct InputSignal
	{

	};

	/// @brief Gives a string representation of an InputType
	std::string InputTypeName(InputType type);

	/// @brief Gives a astring representation of a DirectionalButton
	std::string DirectionalButtonName(DirectionalButton type);

	/// @brief Interface for an input, send signal when it's binding is triggered
	class Input : public std::enable_shared_from_this<Input>
	{
	public:
		virtual ~Input() {};

		virtual void ListenEventAndBindTrigger(const SDL_Event& e, int version = 0) = 0;
		virtual void SetBindings(const Bindings& bindings, int version = 0) = 0;

		virtual std::string GetName() const = 0;
		virtual InputType GetType() const = 0;

		SignalSender<InputSignal> inputSignal;

	protected:
		friend class InputMap;

		virtual void KeyPressed(const SDL_Event& e) {};
		virtual void KeyReleased(const SDL_Event& e) {};
		virtual void MouseButtonPressed(const SDL_Event& e) {};
		virtual void MouseButtonReleased(const SDL_Event& e) {};
		virtual void MouseMoved(const SDL_Event& e) {};
		virtual void ControllerButtonPressed(const SDL_Event& e) {};
		virtual void ControllerButtonReleased(const SDL_Event& e) {};
		virtual void ControllerStickMoved(const SDL_Event& e) {};
		virtual void ControllerTriggerMoved(const SDL_Event& e) {};

		virtual void UpdateEventListened() = 0;
		void NotifyUpdateEvent();
		InputEvent m_eventsListened;

	private:

		friend class InputMap;
		InputMap* m_inputMap = nullptr;
	};
}