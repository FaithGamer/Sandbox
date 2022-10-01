#pragma once

#include "pch.h"
#include <SDL.h>
#include "Vec.h"
#include "Input.h"

namespace sandbox
{
	Input::Input(uptr<Trigger> trigger)
	{
		//Force the input to be initialized with at least one trigger.
		//Once input is set up with a trigger type, it cannot be changed

		m_triggerType = trigger->GetType();
		//m_triggers.emplace_back(std::move(trigger));
	}

	void Input::UpdateTriggers(SDL_Event& e)
	{
		//Check if event matches with any of the current triggers
		///Dispatch events if needed
		/*
		SDL_KEYDOWN
			SDL_KEYUP
			SDL_TEXTINPUT

			SDL_MOUSEMOTION
			SDL_MOUSEBUTTONDOWN
			SDL_MOUSEBUTTONUP
			SDL_MOUSEWHEEL

			SDL_CONTROLLERAXISMOTION
			SDL_CONTROLLERBUTTONDOWN
			SDL_CONTROLLERBUTTONUP
			*/
	}

	void Input::PushTrigger(uptr<Trigger> trigger)
	{
		m_triggers.emplace_back(std::move(trigger));
	}

	void Input::SetTrigger(uptr<Trigger> trigger, int version)
	{
		//Trigger type must match the 
		if (trigger->GetType() != m_triggerType)
		{
			LOG_ERROR("Trying to set trigger that dosen't match the input's trigger type.\ninput: "
				+ m_name + "\nTrigger: " + TriggerTypeName(trigger->GetType()) + "\nVersion: " + std::to_string(version));
			return;
		}
		if (m_triggers.size() < (size_t)version - 1)
		{
			LOG_ERROR("Trying to set trigger version that dosen't exist.\ninput: "
				+ m_name + "\nTrigger: " + TriggerTypeName(trigger->GetType()) + "\nVersion: " + std::to_string(version));
			return;
		}
	}

	///////////////////////////////////////////////////////////////////////////
	/// Bindings the trigger to a specific Key/MouseButton/Controller Button
	///////////////////////////////////////////////////////////////////////////

	void Input::ListenEventAndBindTrigger(SDL_Event& e, int version)
	{
		//If the event.type matches the trigger, bind the event with the trigger
	}

	void Input::BindKeyButton(SDL_Scancode keyButton, int directional)
	{
		if (m_triggerType != TriggerType::button)
		{
			LOG_ERROR("Trying to bind KeyButton with input of the wrong type.\nInput: "
				+ m_name + "\nType: " + TriggerTypeName(m_triggerType));
		}
	}

	void Input::BindMouseButton(Uint8 mouseButton, int directional)
	{
	}

	void Input::BindControllerButton(SDL_GameControllerButton controllerButton, int directional)
	{
	}

	void Input::BindControllerStick(ControllerStick stick)
	{
	}

	int Input::GetTriggersCount() const
	{
		/// How many different triggers can activate this input
		return (int)m_triggers.size();
	}

	uptr<Trigger> Input::GetTrigger(int version) const
	{
		if (version >= (int)m_triggers.size())
		{
			LOG_ERROR("Trying to access version of trigger that dosen't exist, nullptr returned.\ninput: "
				+ m_name + "\ntriggers version count: " + std::to_string(GetTriggersCount())
				+ "\ntargeted trigger version: " + std::to_string(version));
			return nullptr;
		}
		return nullptr;
		//return makeuptr<Trigger>(m_triggers[version]);
	}

	std::string Input::GetName() const
	{
		return m_name;
	}

	TriggerType Input::GetTriggerType() const
	{
		return m_triggerType;
	}
}
