#include "pch.h"
#include "iInput.h"
#include "InputMap.h"

namespace Sandbox
{
	void iInput::NotifyUpdateEvent()
	{
		if (m_inputMap == nullptr)
		{
			LOG_WARN("Input has no InputMap, Input: " + GetName() + ". Can be ignored if you add this input into an InputMap after");
		}
		else
		{
			m_inputMap->UpdateInputEvent(shared_from_this());
		}

	}
}