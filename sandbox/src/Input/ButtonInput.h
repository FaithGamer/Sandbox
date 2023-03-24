#pragma once

#include "Input.h"
#include "Controller.h"
#include "Mouse.h"
#include "Render/Vec.h"

namespace Sandbox
{
	class Bindings;

	/// @brief Binding for a button with keyboard/mouse/controller
	struct Button
	{
		MouseButton mouse = MouseButton::Invalid;
		SDL_Scancode key = SDL_SCANCODE_UNKNOWN;// scancode is a physical position on the keyboard.
		// to retreive it's Keycode on the current layout, use the macro SDL_SCANCODE_TO_KEYCODE
		SDL_GameControllerButton controller = SDL_CONTROLLER_BUTTON_INVALID;
		ControllerTrigger trigger = ControllerTrigger::Undefined;
	};

	enum class DirectionalButton : int
	{
		None = 0,
		Left,
		Top,
		Right,
		Bottom
	};

	class ButtonInput : public Input
	{
	public:
		ButtonInput(std::string name);


		virtual void ListenEventAndBindTrigger(const SDL_Event& e, int version = 0);
		virtual void SetBindings(const Bindings& bondings, int version = 0);

		virtual std::string GetName() const;
		virtual InputType GetType() const;

	protected:
		virtual void KeyPressed(const SDL_Event& e);
		virtual void KeyReleased(const SDL_Event& e);
		virtual void MouseButtonPressed(const SDL_Event& e);
		virtual void MouseButtonReleased(const SDL_Event& e);
		virtual void ControllerButtonPressed(const SDL_Event& e);
		virtual void ControllerButtonReleased(const SDL_Event& e);
		virtual void ControllerTriggerMoved(const SDL_Event& e);

	public:
		/// @brief Set wether or not the input is triggered when the button is pressed
		/// @param triggerOnPress true = yes, false = no
		void SendSignalOnPress(bool triggerOnPress);
		
		/// @brief Set wether or not the input is triggered when the button is released
		/// @param triggerOnRelease true = yes, false = no
		void SendSignalOnRelease(bool triggerOnRelease);

		/// @brief Bind a Key from the keyboard
		/// @param keyButton The scancode of the key
		/// @param version There can be multiple key for the same Input.
		void BindKey(SDL_Scancode keyButton, int version = 0);
		/// @brief Bind a mouse button 
		/// @param mouseButton The mouse button
		/// @param version There can be multiple mouse button for the same Input.
		void BindMouse(MouseButton mouseButton, int version = 0);
		/// @brief Bind to a controller button
		/// @param controllerButton the controller button
		/// @param version There can be multiple controller buttons for the same Input.
		void BindController(SDL_GameControllerButton controllerButton, int version = 0);

		struct State
		{
			bool pressed = false;
		};

	protected:
		virtual void UpdateEventListened();

	private:
		void ReleaseButton();
		void PressButton();

		State m_state;

		Button m_button;

		bool m_sendSignalOnPress;
		bool m_sendSignalOnRelease;
		float m_triggerSensitivity;

		double m_lastTriggerValue;

		std::string m_name;
	};
}