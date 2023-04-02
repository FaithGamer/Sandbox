#pragma once
#include "Sandbox/Input/Input.h"
#include "Sandbox/Input/ButtonInput.h"
#include "Sandbox/Vec.h"

namespace Sandbox
{
	//Using either a set of buttons or a stick to set a direction
	class DirectionalInput 
	{

	public:
		InputType GetType() const { return InputType::Directional; }

		struct DirectionalButton
		{
			Button button;
			Vec3f direction;
		};

		Vec3f direction;

		std::vector<DirectionalButton> keys;
		ControllerStick stick;

	};
}