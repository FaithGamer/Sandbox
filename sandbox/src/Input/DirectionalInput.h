#pragma once
#include "Input.h"
#include "Render/Vec.h"
#include "ButtonInput.h"

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