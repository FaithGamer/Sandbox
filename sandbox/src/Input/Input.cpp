#pragma once

#include "pch.h"
#include <SDL.h>
#include "Vec.h"
#include "Input.h"

namespace sandbox
{
	Input::Input(uptr<Trigger> trigger)
	{
		//Once input is set up with a trigger type, it cannot be changed

		m_triggerType = trigger->GetType();
		m_triggers.emplace_back(std::move(trigger));
	}

	void Input::UpdateState(SDL_Event& e)
	{
		//Check if event match with any of the current triggers
		//Dispatch events if needed
	}

	void Input::ListenAndSetTrigger(int version)
	{
		//SDL_PollEvent and determine given the type of trigger what
		//kind of event to record, exit the Event polling by pressing escape or back
		//Automatically exit after X seconds
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
			LOG_ERROR("Trying to set trigger that dosen't match the input's trigger type, input: " 
				+ m_name + "trigger: " + trigger->GetName() + " Version: " + std::to_string(version));
			return;
		}
		if (m_triggers.size() < (size_t)version - 1)
		{
			LOG_ERROR("Trying to set trigger version that dosen't exist, input: "
				+ m_name + "trigger: " + trigger->GetName() + " Version: " + std::to_string(version));
			return;
		}
	}

	int Input::GetTriggersCount() const
	{
		/// How many different triggers can activate this input
		return (int)m_triggers.size();
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
