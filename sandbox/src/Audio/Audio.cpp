#include "pch.h"
#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio/miniaudio.h"

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
		//Initialize audio memory manager
	/*	ma_engine_config config;
		config.channels = 0;
		config.pDevice*/		//Initialize audio engine
		m_engine = new ma_engine;
		ma_result result = ma_engine_init(NULL, m_engine);
		if (result != MA_SUCCESS)
		{
			LOG_ERROR("Can't initialize audio engine.");
		}

	/*	ma_sound sound;
		ma_sound_init_from_file(m_engine, "assets/audio/checkpoint.wav", 0, NULL, NULL, &sound);
		ma_sound_start(&sound);*/
		/*ma_sound* sound = new ma_sound;*/
		ma_sound* soundPlay = new ma_sound;

		ma_sound_init_from_file(m_engine, "assets/audio/checkpoint.wav", MA_SOUND_FLAG_DECODE, NULL, NULL, soundPlay);


		ma_sound_start(soundPlay);


	}

	void Audio::LoadSound(String path)
	{
		//m_sounds.push_back(new ma_sound);
		//ma_sound_init_from_file(m_engine, path.c_str(), MA_SOUND_FLAG_DECODE, NULL, NULL, m_sounds.back());
	}

	Sound Audio::MakeSound(String soundPath, bool play)
	{
		auto i = Instance();
		ma_sound* masound = new ma_sound;
		ma_sound_init_from_file(i->m_engine, soundPath.c_str(), MA_SOUND_FLAG_DECODE, NULL, NULL, masound);
		if (play)
			ma_sound_start(masound);
		return Sound(masound);
	}

	Audio::~Audio()
	{
		for (auto& sound : m_sounds)
		{
			ma_sound_uninit(sound);
			delete sound;
		}
	}
}

/*
* init from file in assets.
* 
*/