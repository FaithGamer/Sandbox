#pragma once

#include <vector>
#include <SDL.h>
#include "Vec.h"

namespace sandbox
{
	enum class TriggerType
	{
		button = 0,
		pointing,
		directional,
		wheel
	};

	struct ControllerStick
	{
		SDL_GameControllerAxis xAxis = SDL_CONTROLLER_AXIS_LEFTX;
		SDL_GameControllerAxis yAxis = SDL_CONTROLLER_AXIS_LEFTY;
		float xValue = 0;
		float yValue = 0;
		float deadzone = 0;
	};

	class Trigger
	{
	public:
		virtual ~Trigger() {};
		virtual TriggerType GetType() const = 0;
		virtual std::string GetName() const = 0;
	};

	class ButtonTrigger : public Trigger
	{
	public:
		TriggerType GetType() const { return TriggerType::button; }
		std::string GetName() const { return "ButtonTrigger"; }

		bool activateOnRelease = false;
		bool pressed = false;
		Uint8 mouseButton;
		SDL_Scancode key;// scancode is a physical position on the keyboard
						 // to retreive it's Keycode on the current layout, use the macro SDL_SCANCODE_TO_KEYCODE
		SDL_GameControllerButton controllerButton;
	};

	//Pointing with either a stick or the mouse
	class PointingTrigger : public Trigger
	{
	public:
		TriggerType GetType() const { return TriggerType::pointing; }
		std::string GetName() const { return "PointingTrigger"; }

		Vec2f position;
		Vec2f offset;
		ControllerStick stick;
		bool mouse = true;
	};

	//Using either a set of buttons or a stick to set a direction
	class DirectionalTrigger : public Trigger
	{
		//Map a button with a direction
		struct DirectionalButton
		{
			ButtonTrigger button;
			Vec2f direction;
		};
	public:
		TriggerType GetType() const { return TriggerType::directional; }
		std::string GetName() const { return "DirectionalTrigger"; }

		Vec2f direction;
		std::vector<DirectionalButton> keys;
		ControllerStick stick;
	};

	class WheelTrigger : public Trigger
	{
	public:
		TriggerType GetType() const { return TriggerType::wheel; }
		std::string GetName() const { return "WheelTrigger"; }
		bool up;
	};
}