#pragma once

#ifndef SANDBOX_NO_AUDIO
#include "miniaudio/miniaudio.h"
#endif
#include "Sandbox/Core/std_macros.h"

namespace Sandbox
{
	/// @brief Sound handle, can be safely copied.
	/// Must be created with the Audio singleton.
	class Sound
	{
	public:
		
#ifndef SANDBOX_NO_AUDIO
		Sound() = delete;
		Sound(ma_sound* sound);
#else
		Sound() = default;
#endif
		~Sound();
		Sound(Sound& obj);
		Sound& operator=(Sound& obj);
		void Play();
		void Stop();
		void SetLoop(bool loop);
		/// @brief Set the sound volume.
		/// @param volume linear scale between 0.f and 1.f. Above 1.f amplify original sound.
		void SetVolume(float volume);
		bool IsPlaying();
	private:
#ifndef SANDBOX_NO_AUDIO
		int* m_refcount;
		ma_sound* m_sound;
#endif

	};
}