#include "pch.h"
#include "InputHelpers.h"

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

	ControllerStick::ControllerStick()
		: xAxis(SDL_CONTROLLER_AXIS_INVALID), yAxis(SDL_CONTROLLER_AXIS_INVALID)
	{
	}
	ControllerStick::ControllerStick(SDL_GameControllerAxis x, SDL_GameControllerAxis y)
		: xAxis(x), yAxis(y)
	{
	}

	Bindings::Bindings() : mousePointing(true), trigger(ControllerTrigger::Undefined)
	{

	}

	Bindings::Bindings(const nlohmann::json& j) : Bindings()
	{
		//todo
	}

	std::string InputTypeName(InputType type)
	{
		switch (type)
		{
		case InputType::button:			return "Button";		break;
		case InputType::pointing:		return "Pointing";		break;
		case InputType::directional:	return "Directional";	break;
		case InputType::wheel:			return "Wheel";			break;
		case InputType::textual:		return "Textual";		break;
		default:
			LOG_WARN("Trying to get the name of an incorrect InputType");
			return "TypeError";
			break;
		}
	}

	std::string ButtonDirectionTypeName(ButtonDirection type)
	{
		switch (type)
		{
		case ButtonDirection::none:		return "None";			break;
		case ButtonDirection::left:		return "Left";			break;
		case ButtonDirection::top:		return "Top";			break;
		case ButtonDirection::right:	return "Right";			break;
		case ButtonDirection::bottom:	return "Bottom";		break;
		default:
			LOG_WARN("Trying to get the name of an incorrect DirectionType");
			return "TypeError";
			break;
		}
	}

	///////////////////////////////////////
	/// Trigger to Name 
	///////////////////////////////////////

	std::string KeyScancodeName(SDL_Scancode keyButton)
	{
		if ((int)keyButton < 0 || (int)keyButton >= SDL_NUM_SCANCODES)
		{
			LOG_WARN("Trying to get the name of an out of bound Key Scancode: " + std::to_string((int)keyButton));
			return "UnknowKey";
		}
		return std::string(SDL_GetKeyName(SDL_SCANCODE_TO_KEYCODE(keyButton)));
	}

	std::string MouseButtonName(Uint8 mouseButton)
	{
		switch (mouseButton)
		{
		case SDL_BUTTON_LEFT:		return "LeftClick";		break;
		case SDL_BUTTON_RIGHT:		return "RightClick";	break;
		case SDL_BUTTON_MIDDLE:		return "MiddleClick";	break;
		case SDL_BUTTON_X1:			return "X1Click";		break;
		case SDL_BUTTON_X2:			return "X2Click";		break;
		default:
			LOG_WARN("Trying to get the name of an incorrect Mouse Button: " + std::to_string((int)mouseButton));
			return "UnknownMouseButton";
			break;
		}
	}

	std::string ControllerButtonName(SDL_GameControllerButton button)
	{
		if ((int)button < 0 || (int)button >= SDL_CONTROLLER_BUTTON_MAX)
		{
			LOG_WARN("Trying to get the name of an incorrect Controller Button: " + std::to_string((int)button));
			return "UnknowControllerButton";
		}
		return std::string(SDL_GameControllerGetStringForButton(button));
	}

	std::string ControllerStickName(ControllerStick stick)
	{
		if (stick.xAxis == SDL_CONTROLLER_AXIS_LEFTX && stick.yAxis == SDL_CONTROLLER_AXIS_LEFTY)
		{
			return "LeftStick";
		}
		else if (stick.xAxis == SDL_CONTROLLER_AXIS_RIGHTX && stick.yAxis == SDL_CONTROLLER_AXIS_RIGHTY)
		{
			return "RightStick";
		}
		LOG_WARN("Controller axis left/right mismatch, cannot identify a ControllerStick");
		return "UnknownStick";
	}

	///////////////////////////////////////
	/// Name to Trigger
	///////////////////////////////////////

	SDL_Scancode KeyScancode(std::string name)
	{
		auto scancode = SDL_GetScancodeFromName(name.c_str());
		if (scancode == SDL_SCANCODE_UNKNOWN)
		{
			LOG_WARN("Key name unknown, " + LogSDLError(""));
		}
		return scancode;
	}

	Uint8 MouseButton(std::string name)
	{
		if (name == "LeftClick")
			return SDL_BUTTON_LEFT;
		if (name == "RightClick")
			return SDL_BUTTON_RIGHT;
		if (name == "MiddleClick")
			return SDL_BUTTON_MIDDLE;
		if (name == "X1Click")
			return SDL_BUTTON_X1;
		if (name == "X2Click")
			return SDL_BUTTON_X2;

		LOG_WARN("Unknown Mouse Button name: " + name);
		return 0;
	}

	SDL_GameControllerButton ControllerButton(std::string name)
	{
		SDL_GameControllerButton button = SDL_GameControllerGetButtonFromString(name.c_str());
		if (button == SDL_CONTROLLER_BUTTON_INVALID)
		{
			LOG_WARN("Unknown Controller button name:  " + LogSDLError(""));
		}
		return button;
	}

	ControllerStick Stick(std::string name)
	{
		if (name == "LeftStick")
		{
			return ControllerStick(SDL_CONTROLLER_AXIS_LEFTX, SDL_CONTROLLER_AXIS_LEFTY);
		}
		else if (name == "RightStick")
		{
			return ControllerStick(SDL_CONTROLLER_AXIS_RIGHTX, SDL_CONTROLLER_AXIS_RIGHTY);
		}
		LOG_WARN("Unknown Controller Stick name: " + name);
		return ControllerStick();
	}

	ControllerTrigger Trigger(std::string name)
	{
		if (name == "LeftTrigger")
		{
			return ControllerTrigger::Left;
		}
		else if (name == "RightTrigger")
		{
			return ControllerTrigger::Right;
		}
		LOG_WARN("Unknown Controller Trigger name: " + name);
		return ControllerTrigger::Undefined;
	}

}