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
		wheel,
		textual
	};

	static std::string TriggerTypeName(TriggerType type)
	{
		switch (type)
		{
		case TriggerType::button:		return "Button";		break;
		case TriggerType::pointing:		return "Pointing";		break;
		case TriggerType::directional:	return "Directional";	break;
		case TriggerType::wheel:		return "Wheel";			break;
		case TriggerType::textual:		return "Textual";		break;
		default:
			LOG_ERROR("Incorrect TriggerType");
			break;
		}
	}

	struct ControllerStick
	{
		SDL_GameControllerAxis xAxis = SDL_CONTROLLER_AXIS_LEFTX;
		SDL_GameControllerAxis yAxis = SDL_CONTROLLER_AXIS_LEFTY;
		float xValue = 0;
		float yValue = 0;
		float deadzone = 0;
	};

	/// <summary>
	/// Interface for different triggers type.
	/// A trigger reflect one action the player can perform with either it's mouse/keyboard or controller.
	/// ( pressing a button/key, pointing with the mouse or the stick, entering text... )
	/// It records the last state of the performed action ( button pressed/released, pointer position and offset ...)
	/// </summary>
	class Trigger
	{
	public:
		virtual ~Trigger() {};
		virtual TriggerType GetType() const = 0;
	};

	class ButtonTrigger : public Trigger
	{
	public:
		TriggerType GetType() const { return TriggerType::button; }

		//State
		bool pressed = false;

		//Action
		Uint8 mouseButton;
		SDL_Scancode key;// scancode is a physical position on the keyboard
						 // to retreive it's Keycode on the current layout, use the macro SDL_SCANCODE_TO_KEYCODE
		SDL_GameControllerButton controllerButton;

		/// Parameters
		bool dispatchEventOnPress = true;
		bool dispatchEventOnRelease = false;
	};

	//Pointing with either a stick or the mouse
	class PointingTrigger : public Trigger
	{
	public:
		TriggerType GetType() const { return TriggerType::pointing; }

		/// State
		Vec2f position = Vec2f(0, 0);
		Vec2f offset = Vec2f(0, 0);

		/// Action
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

		/// State
		Vec2f direction;

		/// Action
		std::vector<DirectionalButton> keys;
		ControllerStick stick;
	};

	class WheelTrigger : public Trigger
	{
	public:
		TriggerType GetType() const { return TriggerType::wheel; }

		/// State
		bool up;

		//Action is always considered to be the mouse wheel atm
	};
	
	class TextualTrigger : public Trigger
	{
	public:
		TriggerType GetType() const { return TriggerType::textual;  }
		
		/// State
		char text[SDL_TEXTINPUTEVENT_TEXT_SIZE];
	};
}