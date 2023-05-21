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

	typedef enum
	{
		Invalid = SDL_CONTROLLER_BUTTON_INVALID,
		A = SDL_CONTROLLER_BUTTON_A,
		B = SDL_CONTROLLER_BUTTON_B,
		X = SDL_CONTROLLER_BUTTON_X,
		Y = SDL_CONTROLLER_BUTTON_Y,
		Back = SDL_CONTROLLER_BUTTON_BACK,
		Guide = SDL_CONTROLLER_BUTTON_GUIDE,
		Start = SDL_CONTROLLER_BUTTON_START,
		LeftStick = SDL_CONTROLLER_BUTTON_LEFTSTICK,
		RightStick = SDL_CONTROLLER_BUTTON_RIGHTSTICK,
		LeftShoulder = SDL_CONTROLLER_BUTTON_LEFTSHOULDER,
		RightShoulder = SDL_CONTROLLER_BUTTON_RIGHTSHOULDER,
		PadUp = SDL_CONTROLLER_BUTTON_DPAD_UP,
		PadDown = SDL_CONTROLLER_BUTTON_DPAD_DOWN,
		PadLeft = SDL_CONTROLLER_BUTTON_DPAD_LEFT,
		PadRight = SDL_CONTROLLER_BUTTON_DPAD_RIGHT,
		Misc1 = SDL_CONTROLLER_BUTTON_MISC1,    /* Xbox Series X share button, PS5 microphone button, Nintendo Switch Pro capture button, Amazon Luna microphone button */
		Paddle1 = SDL_CONTROLLER_BUTTON_PADDLE1,  /* Xbox Elite paddle P1 */
		Paddle2 = SDL_CONTROLLER_BUTTON_PADDLE2,  /* Xbox Elite paddle P3 */
		Paddle3 = SDL_CONTROLLER_BUTTON_PADDLE3,  /* Xbox Elite paddle P2 */
		Paddle4 = SDL_CONTROLLER_BUTTON_PADDLE4,  /* Xbox Elite paddle P4 */
		TouchPad = SDL_CONTROLLER_BUTTON_TOUCHPAD, /* PS4/PS5 touchpad button */
		ButtonMax = SDL_CONTROLLER_BUTTON_MAX
	} ControllerButton;

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

		constexpr bool operator==(const ControllerStick& other) const
		{
			return xAxis == other.xAxis && yAxis == other.yAxis;
		}

		ControllerAxis xAxis;
		ControllerAxis yAxis;

		static const ControllerStick None;
		static const ControllerStick Left;
		static const ControllerStick Right;
	};

	std::string ControllerButtonName(SDL_GameControllerButton button);
	std::string ControllerStickName(ControllerStick stick);
	std::string ControllerTriggerName(ControllerTrigger trigger);

	SDL_GameControllerButton ControllerButtonFromName(std::string name);
	ControllerStick ControllerStickFromName(std::string name);
	ControllerTrigger ControllerTriggerFromName(std::string name);
}