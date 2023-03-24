#pragma once
#include "Input.h"
#include "Render/Vec.h"
#include "Controller.h"

//Pointing with either a stick or the mouse
namespace Sandbox
{
	class PointingInput 
	{
	public:
		InputType GetType() const { return InputType::Pointing; }

		struct Data
		{
			Vec2f position = Vec2f(0, 0);
			Vec2f offset = Vec2f(0, 0);
		};
	
		Data m_data;


		ControllerStick m_stick;
		bool m_mouse = true;
	};
}