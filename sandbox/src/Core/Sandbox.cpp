#include "pch.h"
#include "Sandbox.h"
#include "Systems.h"

namespace Sandbox
{
	bool play = false;
	void Start()
	{
		play = true;
		while (play)
		{
			Systems::Get()->Update();
		}

		Systems::Get()->Kill();
	}

	void Stop()
	{
		play = false;
	}
}

