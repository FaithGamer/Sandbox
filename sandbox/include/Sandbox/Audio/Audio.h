#pragma once

#include "Sandbox/Internal/Singleton.h"

#include "miniaudio/miniaudio.h"
#include "Sandbox/Audio/Sound.h"

namespace Sandbox
{
	class Audio : public Singleton<Audio>
	{
	public:

		Audio();
		~Audio();
		void Init();
		/// @brief For internal use only. (Assets loading)
		/// @param path 
		void LoadSound(String path);
		/// @brief Instance handle for the sound located at path.
		/// The returned handle must be kept alive as long as you want the sound to play.
		/// The sound has already been loaded, path is just used as a hash key in the memory manager.
		/// To play multiple times the same sound at the same time, call this function multiple times.
		/// @param path The full path of the sound
		/// @play Play the sound right away or not.
		/// @return Handle to the sound being played.
		static Sound MakeSound(String path, bool play = true);

	private:
		friend sptr<Audio> Singleton<Audio>::Instance();
		friend void Singleton<Audio>::Kill();

		ma_engine* m_engine;
		std::vector<ma_sound*> m_sounds;
	};
}