#pragma once

#include "Sandbox/Input/Input.h"
#include "Sandbox/Input/Keyboard.h"
#include "Sandbox/Input/Controller.h"
#include "Sandbox/Input/Mouse.h"
#include "Sandbox/Vec.h"


namespace Sandbox
{
	class Bindings;

	/// @brief Binding for a button with keyboard/mouse/controller
	struct Button
	{
		MouseButton mouse = MouseButton::Invalid;
		KeyScancode key = KeyScancode::Unknown;// scancode is a physical position on the keyboard.
		// to retreive it's Keycode on the current layout, use the macro SDL_SCANCODE_TO_KEYCODE
		ControllerButton controller = ControllerButton::Invalid;
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
		

		virtual void ListenEventAndBindTrigger(const SDL_Event& e, int version = 0);
		virtual void SetBindings(const Bindings& bondings, int version = 0);

		virtual std::string GetName() const;
		virtual InputType GetType() const;

	protected:
		virtual bool KeyPressed(const SDL_Event& e);
		virtual bool KeyReleased(const SDL_Event& e);
		virtual bool MouseButtonPressed(const SDL_Event& e);
		virtual bool MouseButtonReleased(const SDL_Event& e);
		virtual bool ControllerButtonPressed(const SDL_Event& e);
		virtual bool ControllerButtonReleased(const SDL_Event& e);
		virtual bool ControllerTriggerMoved(const SDL_Event& e);

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
		void BindKey(KeyScancode keyButton, int version = 0);
		/// @brief Bind a mouse button 
		/// @param mouseButton The mouse button
		/// @param version There can be multiple mouse button for the same Input.
		void BindMouse(MouseButton mouseButton, int version = 0);
		/// @brief Bind to a controller button
		/// @param controllerButton the controller button
		/// @param version There can be multiple controller buttons for the same Input.
		void BindController(ControllerButton controllerButton, int version = 0);

		struct State
		{
			bool pressed = false;
		};

	protected:
		virtual void UpdateEventListened();

	private:
		friend InputMap;
		ButtonInput(std::string name);

		bool ReleaseButton();
		bool PressButton();

		State m_state;

		Button m_button;

		bool m_sendSignalOnPress;
		bool m_sendSignalOnRelease;
		float m_triggerSensitivity;

		double m_lastTriggerValue;

		std::string m_name;
	};
}