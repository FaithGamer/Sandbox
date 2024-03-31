#include "pch.h"
#include "Sandbox/Input/Input.h"
#include "Sandbox/Input/InputMap.h"
#include "Sandbox/Input/ButtonInput.h"

namespace Sandbox
{

	bool InputEvent::operator==(const InputEvent& other)
	{
		if (keyButton != other.keyButton
			|| keyText != other.keyText
			|| mouseButton != other.mouseButton
			|| mouseMovement != other.mouseMovement
			|| mouseWheel != other.mouseWheel
			|| controllerButton != other.controllerButton
			|| controllerStick != other.controllerStick
			|| controllerTrigger != other.controllerTrigger)
		{
			return false;
		}
		return true;
	}
	bool InputEvent::operator!=(const InputEvent& other)
	{
		if (keyButton != other.keyButton
			|| keyText != other.keyText
			|| mouseButton != other.mouseButton
			|| mouseMovement != other.mouseMovement
			|| mouseWheel != other.mouseWheel
			|| controllerButton != other.controllerButton
			|| controllerStick != other.controllerStick
			|| controllerTrigger != other.controllerTrigger)
		{
			return true;
		}
		return false;
	}

	std::string InputTypeName(InputType type)
	{
		switch (type)
		{
		case InputType::Button:			return "Button";		break;
		case InputType::Directional:	return "Directional";	break;
		case InputType::Textual:		return "Textual";		break;
		default:
			LOG_WARN("Trying to get the name of an incorrect InputType");
			return "TypeError";
			break;
		}
	}

	/* ____ Input ____ */

	void Input::OnEventListenedUpdated()
	{
		if (m_inputMap == nullptr)
		{
			LOG_ERROR("Input has no InputMap, Input: " + GetName());
		}
		else
		{
			m_inputMap->OnInputEventModified(shared_from_this());
		}

	}


}