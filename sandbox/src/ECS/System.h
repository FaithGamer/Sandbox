#pragma once
#include "std_macros.h"
#include "Core/Time.h"

namespace Sandbox
{
	class System
	{
	public:
		virtual ~System() {};


		virtual void Update(Time t){};
		virtual void FixedUpdate() {};

	private:


	};
}