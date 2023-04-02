#pragma once

#include "Sandbox/Input/ButtonInput.h"

/// @brief Every possible bindings, can be used to set binding on an Input wich will ignore the adequate fields
namespace Sandbox
{
	struct Bindings
	{
		Bindings();

		Button button;

		Button leftButton;
		Button topButton;
		Button rightButton;
		Button botButton;

		ControllerStick stick;
		ControllerTrigger trigger;

		bool mousePointing = true;
	};
}