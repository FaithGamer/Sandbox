#include "pch.h"
#include "Sandbox/Input/Keyboard.h"
#include "Sandbox/Core/Log.h"

namespace Sandbox
{
	std::string KeyScancodeName(KeyScancode keyButton)
	{
		if ((int)keyButton < 0 || (int)keyButton >= SDL_NUM_SCANCODES)
		{
			LOG_ERROR("Trying to get the name of an out of bound Key Scancode: " + std::to_string((int)keyButton));
			return "UnknowKey";
		}
		return std::string(SDL_GetKeyName(SDL_SCANCODE_TO_KEYCODE((SDL_Scancode)keyButton)));
	}

	KeyScancode KeyScancodeFromName(std::string name)
	{
		KeyScancode scancode = (KeyScancode)SDL_GetScancodeFromName(name.c_str());
		if (scancode == KeyScancode::Unknown)
		{
			LOG_ERROR("Key name unknown, " + LogSDLError(""));
		}
		return scancode;
	}
}