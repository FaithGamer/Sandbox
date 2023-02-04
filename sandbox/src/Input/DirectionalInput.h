#pragma once
#include "iInput.h"
#include "Vec.h"

namespace Sandbox
{
	//Using either a set of buttons or a stick to set a direction
	class DirectionalInput 
	{

	public:
		InputType GetType() const { return InputType::directional; }

		//Map a button with a direction
		struct DirectionalButton
		{
			Button button;
			Vec2f direction;
		};

		/// State
		Vec2f direction;

		/// Action
		std::vector<DirectionalButton> keys;
		ControllerStick stick;

	};
}