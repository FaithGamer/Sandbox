#include "pch.h"
#include "Input.h"
#include "InputMap.h"
#include "ButtonInput.h"

namespace Sandbox
{

	bool InputEvent::operator==(const InputEvent& other)
	{
		if (keyButton != other.keyButton
			|| keyText != other.keyText
			|| mouseButton != other.mouseButton
			|| mouseMovement != other.mouseMovement
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
		case InputType::Pointing:		return "Pointing";		break;
		case InputType::Directional:	return "Directional";	break;
		case InputType::Scroll:			return "Scroll";		break;
		case InputType::Textual:		return "Textual";		break;
		default:
			LOG_WARN("Trying to get the name of an incorrect InputType");
			return "TypeError";
			break;
		}
	}

	std::string DirectionalButtonName(DirectionalButton type)
	{
		switch (type)
		{
		case DirectionalButton::None:		return "None";			break;
		case DirectionalButton::Left:		return "Left";			break;
		case DirectionalButton::Top:		return "Top";			break;
		case DirectionalButton::Right:	return "Right";			break;
		case DirectionalButton::Bottom:	return "Bottom";		break;
		default:
			LOG_WARN("Trying to get the name of an incorrect DirectionalButton");
			return "TypeError";
			break;
		}
	}

	/* ____ Input ____ */

	void Input::NotifyUpdateEvent()
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