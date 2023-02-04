#pragma once
#include "iInput.h"
#include "Vec.h"

//Pointing with either a stick or the mouse
namespace Sandbox
{
	class PointingInput 
	{
	public:
		InputType GetType() const { return InputType::pointing; }

		struct Data
		{
			Vec2f position = Vec2f(0, 0);
			Vec2f offset = Vec2f(0, 0);
		};
		/// State
		Data m_data;

		/// Action
		ControllerStick m_stick;
		bool m_mouse = true;
	};
}