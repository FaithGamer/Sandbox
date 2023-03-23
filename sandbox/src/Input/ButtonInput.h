#pragma once

#include "iInput.h"
#include "Render/Vec.h"

/**
 * \file ButtonInput.h
 * 
 * Button Input like a key or controller button.
 * Can be either pressed or released
 */
namespace Sandbox
{
	class ButtonInput : public iInput
	{
	public:
		ButtonInput(std::string name);

		//iInput
		virtual void KeyPressed(const SDL_Event& e);
		virtual void KeyReleased(const SDL_Event& e);
		virtual void MouseButtonPressed(const SDL_Event& e);
		virtual void MouseButtonReleased(const SDL_Event& e);
		virtual void ControllerButtonPressed(const SDL_Event& e);
		virtual void ControllerButtonReleased(const SDL_Event& e);
		virtual void ControllerTriggerMoved(const SDL_Event& e);

		virtual void ListenEventAndBindTrigger(const SDL_Event& e, int version = 0);
		virtual void SetBindings(const Bindings& bondings, int version = 0);

		virtual std::string GetName() const;
		virtual InputType GetType() const;

		//ButtonInput

	
		/// @brief Set wether or not the input is triggered when the button is pressed
		/// @param triggerOnPress true = yes, false = no
		void SetTriggerOnPress(bool triggerOnPress);
		
		/// @brief Set wether or not the input is triggered when the button is released
		/// @param triggerOnRelease true = yes, false = no
		void SetTriggerOnRelease(bool triggerOnRelease);

		/// @brief Bind a Key from the keyboard
		/// @param keyButton The scancode of the key
		/// @param version There can be multiple key for the same Input.
		void BindKey(SDL_Scancode keyButton, int version = 0);
		void BindMouse(Uint8 mouseButton, int version = 0);
		void BindController(SDL_GameControllerButton controllerButton, int version = 0);

		struct State
		{
			bool pressed = false;
		};

	protected:
		//From iInput
		virtual void UpdateEventListened();

	private:
		//Proper tu ButtonInput
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