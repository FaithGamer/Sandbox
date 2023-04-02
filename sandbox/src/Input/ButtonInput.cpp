#include "pch.h"
#include "Sandbox/Math.h"
#include "Sandbox/Input/ButtonInput.h"
#include "Sandbox/Input/Bindings.h"

namespace Sandbox
{

	ButtonInput::ButtonInput(std::string name) : 
		m_name(name), m_sendSignalOnPress(true), m_sendSignalOnRelease(false), m_triggerSensitivity(0.5f), m_lastTriggerValue(0.f)
	{

	}

	bool ButtonInput::KeyPressed(const SDL_Event& e)
	{
		if (e.key.keysym.scancode == m_button.key)
		{
			PressButton();
			return true;
		}
		return false;
	}

	bool ButtonInput::KeyReleased(const SDL_Event& e)
	{
		if (e.key.keysym.scancode == m_button.key)
		{
			return ReleaseButton();
		}
		return false;
	}

	bool ButtonInput::MouseButtonPressed(const SDL_Event& e)
	{
		if (e.button.button == (Uint8)m_button.mouse)
		{
			return PressButton();
		}
		return false;
	}

	bool ButtonInput::MouseButtonReleased(const SDL_Event& e)
	{
		if (e.button.button == (Uint8)m_button.mouse)
		{
			return ReleaseButton();
		}
		return false;
	}

	bool ButtonInput::ControllerButtonPressed(const SDL_Event& e)
	{
		if ((SDL_GameControllerButton)e.cbutton.button == (SDL_GameControllerButton)m_button.controller)
		{
			return PressButton();
		}
		return false;
	}

	bool ButtonInput::ControllerButtonReleased(const SDL_Event& e)
	{
		if ((SDL_GameControllerButton)e.cbutton.button == (SDL_GameControllerButton)m_button.controller)
		{
			return ReleaseButton();
		}
		return false;
	}

	bool ButtonInput::ControllerTriggerMoved(const SDL_Event& e)
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
					return PressButton();
				}
			}
			else
			{
				//trigger released
				if (m_state.pressed)
				{
					return ReleaseButton();
				}
			}
		}
		return false;
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
		return InputType::Button;
	}
	
	void ButtonInput::SendSignalOnPress(bool triggerOnPress)
	{
		m_sendSignalOnPress = triggerOnPress;
	}

	void ButtonInput::SendSignalOnRelease(bool triggerOnRelease)
	{
		m_sendSignalOnRelease = triggerOnRelease;
	}

	void ButtonInput::BindKey(SDL_Scancode keyButton, int version)
	{
		m_button.key = keyButton;
		UpdateEventListened();
	}

	void ButtonInput::BindMouse(MouseButton mouseButton, int version)
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

		if (m_button.mouse != MouseButton::Invalid)
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
			NotifyEventListenedModified();
		}
	}

	bool ButtonInput::PressButton()
	{
		if (!m_state.pressed)
		{
			m_state.pressed = true;
			if (m_sendSignalOnPress)
			{
				inputSignal.SendSignal(InputSignal());
				return true;
			}
		}
		return false;
	}

	bool ButtonInput::ReleaseButton()
	{
		if (m_state.pressed)
		{
			m_state.pressed = false;
			if (m_sendSignalOnRelease)
			{
				inputSignal.SendSignal(InputSignal());
				return true;
			}
		} 
		return false;
	}

}