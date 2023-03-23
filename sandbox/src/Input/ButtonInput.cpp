#include "pch.h"
#include "ButtonInput.h"
#include "Math/Math.h"

namespace Sandbox
{

	ButtonInput::ButtonInput(std::string name) : 
		m_name(name), m_sendSignalOnPress(true), m_sendSignalOnRelease(false), m_triggerSensitivity(0.5f), m_lastTriggerValue(0.f)
	{

	}

	void ButtonInput::KeyPressed(const SDL_Event& e)
	{
		if (e.key.keysym.scancode == m_button.key)
		{
			PressButton();
		}
	}

	void ButtonInput::KeyReleased(const SDL_Event& e)
	{
		if (e.key.keysym.scancode == m_button.key)
		{
			ReleaseButton();
		}
	}

	void ButtonInput::MouseButtonPressed(const SDL_Event& e)
	{
		if (e.button.button == m_button.mouse)
		{
			PressButton();
		}
	}

	void ButtonInput::MouseButtonReleased(const SDL_Event& e)
	{
		if (e.button.button == m_button.mouse)
		{
			ReleaseButton();
		}
	}

	void ButtonInput::ControllerButtonPressed(const SDL_Event& e)
	{
		if ((SDL_GameControllerButton)e.cbutton.button == m_button.controller)
		{
			PressButton();
		}
	}

	void ButtonInput::ControllerButtonReleased(const SDL_Event& e)
	{
		if ((SDL_GameControllerButton)e.cbutton.button == m_button.controller)
		{
			ReleaseButton();
		}
	}

	void ButtonInput::ControllerTriggerMoved(const SDL_Event& e)
	{
		if (e.caxis.axis == (Uint8)m_button.trigger)
		{
			//Scale trigger axis value to 0 to 1
			double value = Math::ScaleRangeTo((float)e.caxis.value, 0.f, 1.f, 0.f, 32767.f);
			
			if (value > 1.f - m_triggerSensitivity)
			{
				//trigger pressed
				if (!m_state.pressed)
				{
					PressButton();
				}
			}
			else
			{
				//trigger released
				if (m_state.pressed)
				{
					ReleaseButton();
				}
			}
		}
	}


	void ButtonInput::ListenEventAndBindTrigger(const SDL_Event& e, int version)
	{
		//Keyboard
		if (e.type == SDL_KEYUP)
		{
			m_button.key = e.key.keysym.scancode;
		}
		//Controller
		else if (e.type == SDL_CONTROLLERBUTTONUP)
		{
			m_button.controller = (SDL_GameControllerButton)e.cbutton.button;
		}
	}

	void ButtonInput::SetBindings(const Bindings& bindings, int version)
	{
		m_button = bindings.button;
		UpdateEventListened();
	}


	std::string ButtonInput::GetName() const
	{
		return m_name;
	}

	InputType ButtonInput::GetType() const
	{
		return InputType::button;
	}
	
	void ButtonInput::SetTriggerOnPress(bool triggerOnPress)
	{
		m_sendSignalOnPress = triggerOnPress;
	}

	void ButtonInput::SetTriggerOnRelease(bool triggerOnRelease)
	{
		m_sendSignalOnRelease = triggerOnRelease;
	}

	void ButtonInput::BindKey(SDL_Scancode keyButton, int version)
	{
		m_button.key = keyButton;
		UpdateEventListened();
	}

	void ButtonInput::BindMouse(Uint8 mouseButton, int version)
	{
		m_button.mouse = mouseButton;
		UpdateEventListened();
	}

	void ButtonInput::BindController(SDL_GameControllerButton controllerButton, int version)
	{
		m_button.controller = controllerButton;
		UpdateEventListened();
	}

	void ButtonInput::UpdateEventListened()
	{

		InputEvent newEvents;

		newEvents.keyText = false;
		newEvents.mouseMovement = false;

		if (m_button.key != SDL_SCANCODE_UNKNOWN)
			newEvents.keyButton = true;
		else
			newEvents.keyButton = false;

		if (m_button.mouse != 0)
			newEvents.mouseButton = true;
		else
			newEvents.mouseButton = false;

		if (m_button.controller != SDL_CONTROLLER_BUTTON_INVALID)
			newEvents.controllerButton = true;
		else
			newEvents.controllerButton = false;

		if ((SDL_GameControllerAxis)m_button.trigger != SDL_CONTROLLER_AXIS_INVALID)
			newEvents.controllerTrigger = true;
		else
			newEvents.controllerTrigger = false;

		if (m_eventsListened != newEvents)
		{
			m_eventsListened = newEvents;
			NotifyUpdateEvent();
		}
	}

	void ButtonInput::PressButton()
	{
		if (!m_state.pressed)
		{
			m_state.pressed = true;
			if (m_sendSignalOnPress)
			{
				SendSignal(m_state);
			}
		}
	}

	void ButtonInput::ReleaseButton()
	{
		if (m_state.pressed)
		{
			m_state.pressed = false;
			if (m_sendSignalOnRelease)
			{
				SendSignal(m_state);
			}
		} 
	}

}