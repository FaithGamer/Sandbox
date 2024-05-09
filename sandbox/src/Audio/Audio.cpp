#include "pch.h"
#ifndef SANDBOX_NO_AUDIO
#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio/miniaudio.h"
#endif

#include "Sandbox/Audio/Audio.h"
#include "Sandbox/Core/Log.h"
#include "Sandbox/Audio/Sound.h"

namespace Sandbox
{
	Audio::Audio()
	{
	}

	void Audio::Init()
	{
#ifndef SANDBOX_NO_AUDIO
		//Initialize audio engine
		m_engine = new ma_engine;
		ma_result result = ma_engine_init(NULL, m_engine);
		if (result != MA_SUCCESS)
		{
			LOG_ERROR("Can't initialize audio engine.");
		}
#endif
	}

	void Audio::LoadSound(String path)
	{
#ifndef SANDBOX_NO_AUDIO
		m_sounds.push_back(new ma_sound);
		ma_sound_init_from_file(m_engine, path.c_str(), MA_SOUND_FLAG_DECODE, NULL, NULL, m_sounds.back());
#endif
	}

	Sound Audio::MakeSound(String soundPath, bool play)
	{
#ifndef SANDBOX_NO_AUDIO
		auto i = Instance();
		ma_sound* masound = new ma_sound;
		ma_sound_init_from_file(i->m_engine, soundPath.c_str(), MA_SOUND_FLAG_DECODE, NULL, NULL, masound);
		if (play)
			ma_sound_start(masound);
		return Sound(masound);
#else
		return Sound();
#endif
	}

	Audio::~Audio()
	{
#ifndef SANDBOX_NO_AUDIO
		for (auto& sound : m_sounds)
		{
			ma_sound_uninit(sound);
			delete sound;
		}
#endif
	}
}

/*
* init from file in assets.
* 
*/