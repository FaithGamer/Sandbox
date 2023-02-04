#pragma once

#include <string>
#include <SDL.h>
#include "Log.h"
#include <nlohmann/json.hpp>

namespace Sandbox
{
	enum class InputType : int
	{
		button = 0,
		pointing,
		directional,
		wheel,
		textual
	};

	enum class ButtonDirection : int
	{
		none = 0,
		left,
		top,
		right,
		bottom
	};

	/// <summary>
	/// What events does an input listen to
	/// </summary>
	struct InputEvent
	{
		bool keyButton = false;
		bool keyText = false;
		bool mouseButton = false;
		bool mouseMovement = false;
		bool controllerButton = false;
		bool controllerStick = false;
		bool controllerTrigger = false;

		bool operator==(const InputEvent& other);
		bool operator!=(const InputEvent& other);
	};

	/// <summary>
	/// Two controller axis to identify a Stick
	/// </summary>
	struct ControllerStick
	{
		ControllerStick();
		ControllerStick(SDL_GameControllerAxis x, SDL_GameControllerAxis y);
		SDL_GameControllerAxis xAxis;
		SDL_GameControllerAxis yAxis;
	};

	enum class ControllerTrigger
	{
		Undefined = SDL_CONTROLLER_AXIS_INVALID,
		Left = SDL_CONTROLLER_AXIS_TRIGGERLEFT,
		Right = SDL_CONTROLLER_AXIS_TRIGGERRIGHT
	};

	/// <summary>
	/// Binding for a button with keyboard/mouse/controller
	/// </summary>
	struct Button
	{
		Uint8 mouse = 0;
		SDL_Scancode key = SDL_SCANCODE_UNKNOWN;// scancode is a physical position on the keyboard.
												// to retreive it's Keycode on the current layout, use the macro SDL_SCANCODE_TO_KEYCODE
		SDL_GameControllerButton controller = SDL_CONTROLLER_BUTTON_INVALID;
		ControllerTrigger trigger = ControllerTrigger::Undefined;
	};

	/// <summary>
	/// Every possible bindings, 
	/// can be used to set binding on an Input wich will ignore the adequate fields
	/// </summary>
	struct Bindings
	{
		Bindings();
		Bindings(const nlohmann::json& j);

		Button button;

		Button leftButton;
		Button topButton;
		Button rightButton;
		Button botButton;

		ControllerStick stick;
		ControllerTrigger trigger;

		bool mousePointing = true;
	};

	///////////////////////////////////////
	/// Custom Type names
	///////////////////////////////////////

	std::string InputTypeName(InputType type);
	std::string ButtonDirectionTypeName(ButtonDirection type);

	///////////////////////////////////////
	/// Trigger to Name 
	///////////////////////////////////////

	std::string KeyScancodeName(SDL_Scancode keyButton);
	std::string MouseButtonName(Uint8 mouseButton);
	std::string ControllerButtonName(SDL_GameControllerButton button);
	std::string ControllerStickName(ControllerStick stick);

	///////////////////////////////////////
	/// Name to Trigger
	///////////////////////////////////////

	SDL_Scancode KeyScancode(std::string name);
	Uint8 MouseButton(std::string name);
	SDL_GameControllerButton ControllerButton(std::string name);
	ControllerStick Stick(std::string name);
	ControllerTrigger Trigger(std::string name);
}