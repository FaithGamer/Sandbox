#pragma once
#include <SDL/SDL.h>

namespace Sandbox
{
	enum class ControllerAxis
	{

		Invalid = SDL_CONTROLLER_AXIS_INVALID,
		LeftX = SDL_CONTROLLER_AXIS_LEFTX,
		LeftY = SDL_CONTROLLER_AXIS_LEFTY,
		RightX = SDL_CONTROLLER_AXIS_RIGHTX,
		RightY = SDL_CONTROLLER_AXIS_RIGHTY,
		TriggerLeft = SDL_CONTROLLER_AXIS_TRIGGERLEFT,
		TriggerRight = SDL_CONTROLLER_AXIS_TRIGGERRIGHT,
		AxisMax = SDL_CONTROLLER_AXIS_MAX,

	};

	enum class ControllerButton
	{

	};

	enum class ControllerTrigger
	{
		Undefined = SDL_CONTROLLER_AXIS_INVALID,
		Left = SDL_CONTROLLER_AXIS_TRIGGERLEFT,
		Right = SDL_CONTROLLER_AXIS_TRIGGERRIGHT
	};

	/// @brief Two controller axis to identify a Stick
	struct ControllerStick
	{
		ControllerStick();
		constexpr ControllerStick(ControllerAxis x, ControllerAxis y);

		constexpr static ControllerStick Left()
		{
			return ControllerStick(ControllerAxis::LeftX, ControllerAxis::LeftY);
		}
		constexpr static ControllerStick Right()
		{
			return ControllerStick(ControllerAxis::RightX, ControllerAxis::RightY);
		}
		constexpr bool operator==(const ControllerStick& other)
		{
			return xAxis == other.xAxis && yAxis == other.yAxis;
		}

		ControllerAxis xAxis;
		ControllerAxis yAxis;
	};

	

	std::string ControllerButtonName(SDL_GameControllerButton button);
	std::string ControllerStickName(ControllerStick stick);
	std::string ControllerTriggerName(ControllerTrigger trigger);

	SDL_GameControllerButton ControllerButtonFromName(std::string name);
	ControllerStick ControllerStickFromName(std::string name);
	ControllerTrigger ControllerTriggerFromName(std::string name);
}