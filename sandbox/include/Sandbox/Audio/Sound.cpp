#include "pch.h"
#include "Sound.h"

namespace Sandbox
{
#ifndef SANDBOX_NO_AUDIO
	Sound::Sound(ma_sound* sound) : m_sound(sound)
	{

		m_refcount = new int(1);
	}
#endif

	Sound::~Sound()
	{
#ifndef SANDBOX_NO_AUDIO
		*m_refcount = *m_refcount - 1;
		if (*m_refcount <= 0)
		{
			ma_sound_uninit(m_sound);
			delete m_sound;
			delete m_refcount;
		}
#endif
	}

	Sound::Sound(Sound& obj)
	{
#ifndef SANDBOX_NO_AUDIO
		m_sound = obj.m_sound;
		m_refcount = obj.m_refcount;
		*m_refcount = *m_refcount + 1;
#endif
	}

	Sound& Sound::operator=(Sound& obj)
	{
#ifndef SANDBOX_NO_AUDIO
		m_sound = obj.m_sound;
		m_refcount = obj.m_refcount;
		*m_refcount = *m_refcount + 1;
#endif
		return *this;

	}

	void Sound::Play()
	{
#ifndef SANDBOX_NO_AUDIO
		ma_sound_start(m_sound);
#endif
	}

	void Sound::Stop()
	{
#ifndef SANDBOX_NO_AUDIO
		ma_sound_stop(m_sound);
#endif
	}

	void Sound::SetLoop(bool loop)
	{
#ifndef SANDBOX_NO_AUDIO
		ma_sound_set_looping(m_sound, (ma_bool32)loop);
#endif
	}

	void Sound::SetVolume(float volume)
	{
#ifndef SANDBOX_NO_AUDIO
		ma_sound_set_volume(m_sound, volume);
#endif
	}

	bool Sound::IsPlaying()
	{
#ifndef SANDBOX_NO_AUDIO
		return ma_sound_is_playing(m_sound);
#endif
		return false;
	}




}