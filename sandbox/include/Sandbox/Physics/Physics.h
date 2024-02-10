#pragma once

#include "Sandbox/Singleton.h"
#include "Sandbox/Physics/Raycast.h"
namespace Sandbox
{
	class Physics : public Singleton<Physics>
	{
	public:

	private:
		friend Singleton<Physics>;
	};
}