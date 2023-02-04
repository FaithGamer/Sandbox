#pragma once

#include "iInput.h"
#include "Vec.h"

namespace Sandbox
{
	class ButtonInput : public iInput
	{
	public:
		ButtonInput(std::string name);

		//From iInput
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

		//Proper to ButtonInput
		void SetTriggerOnPress(bool triggerOnPress);
		void SetTriggerOnRelease(bool triggerOnRelease);
		void BindKey(SDL_Scancode keyButton, int version = 0);
		void BindMouse(Uint8 mouseButton, int version = 0);
		void BindController(SDL_GameControllerButton controllerButton, int version = 0);

		static struct State
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