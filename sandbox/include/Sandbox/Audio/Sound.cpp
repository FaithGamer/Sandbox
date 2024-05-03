#include "pch.h"
#include "Sound.h"

namespace Sandbox
{
	Sound::Sound(ma_sound* sound) : m_sound(sound)
	{
		m_refcount = new int(1);
	}

	Sound::~Sound()
	{
		*m_refcount = *m_refcount - 1;
		if (*m_refcount <= 0)
		{
			ma_sound_uninit(m_sound);
			delete m_sound;
			delete m_refcount;
		}
	}

	Sound::Sound(Sound& obj)
	{
		m_sound = obj.m_sound;
		m_refcount = obj.m_refcount;
		*m_refcount = *m_refcount + 1;
	}

	Sound& Sound::operator=(Sound& obj)
	{
		m_sound = obj.m_sound;
		m_refcount = obj.m_refcount;
		*m_refcount = *m_refcount + 1;
		return *this;
	}

	void Sound::Play()
	{
		ma_sound_start(m_sound);
	}

	void Sound::Stop()
	{
		ma_sound_stop(m_sound);
	}

	void Sound::SetLoop(bool loop)
	{
		ma_sound_set_looping(m_sound, (ma_bool32)loop);
	}

	void Sound::SetVolume(float volume)
	{
		ma_sound_set_volume(m_sound, volume);
	}

	bool Sound::IsPlaying()
	{
		return ma_sound_is_playing(m_sound);
	}




}