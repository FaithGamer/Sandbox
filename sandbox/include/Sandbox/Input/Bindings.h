#pragma once

#include "Sandbox/Input/Keyboard.h"
#include "Sandbox/Input/Controller.h"
#include "Sandbox/Input/Mouse.h"
#include "Sandbox/Vec.h"

/// @brief Every possible bindings, can be used to set binding on an Input wich will ignore the adequate fields
namespace Sandbox
{
	//To do: serialization

	struct Button
	{
		Button() {}
		Button(KeyScancode Key) : key(Key) {}
		Button(MouseButton Mouse) : mouse(Mouse) {}
		Button(ControllerButton Controller) : controller(Controller) {}
		Button(ControllerTrigger Trigger) : trigger(Trigger) {}

		MouseButton mouse = MouseButton::Invalid;
		KeyScancode key = KeyScancode::Unknown;// scancode is a physical position on the keyboard.
		// to retreive it's Keycode on the current keyboard layout, use the macro SDL_SCANCODE_TO_KEYCODE
		ControllerButton controller = ControllerButton::Invalid;
		ControllerTrigger trigger = ControllerTrigger::Undefined;
	};

	struct DirectionalButton
	{
		DirectionalButton() {}
		DirectionalButton(Button Button, Vec2f Direction) : button(Button), direction(Direction) {}

		Button button;
		Vec2f direction = Vec2f(0, 0);
		bool pressed = false;
	};

	struct Direction
	{
		Direction() {}
		Direction(std::vector<DirectionalButton> Buttons) : buttons(Buttons) {}
		Direction(ControllerStick Stick) : stick(Stick) {}

		std::vector<DirectionalButton> buttons;
		ControllerStick stick;
	};

	////////////////
	/// Bindings ///
	////////////////

	struct DirectionalBindings
	{	
		std::vector<Direction> directions;
	};

	struct ButtonBindings
	{
		std::vector<Button> buttons;
	};
}