/* Copyright (c) 2010-2011 Andrée Ekroth
 * See the file LICENSE.txt for copying permission.
 */

#ifndef KKE_SOUNDMANAGER_HPP
#define KKE_SOUNDMANAGER_HPP

#include "Interface/INoncopyable.hpp"
#include "Sound/Sound.hpp"
#include "Utilities/Singleton.hpp"

#include <AL/alure.h>
#include <vector>

typedef unsigned int uint;

namespace kke
{
	class Sound;

	/**
	* @brief Manages OpenAL sound.
	**/
	class SoundManager : public Singleton<SoundManager>
	{
		friend class Singleton<SoundManager>;
		
	public:

		const glm::vec3& GetListenerPos();
		const glm::vec3& GetListenerVel();

		void SetListenerPos(const glm::vec3& position);
		void SetListenerVel(const glm::vec3& velocity);

		void MoveListenerPos(const glm::vec3& delta);
		void MoveListenerVel(const glm::vec3& delta);

		void Update(float elapsed);

		void Play(Sound* sound, bool fade = false);
		void Pause(Sound* sound, bool fade = false);
		void Stop(Sound* sound, bool fade = false);
		void StopAll(bool fade = false);

		void RemoveBuffer(SoundBuffer* buffer);
		void Remove(Sound* sound);
		void RemoveAll();
		
		static void CallbackStop(void* userData, ALuint source);
		static bool GetALErrors();
		
	private:
		SoundManager();
		~SoundManager();
		
	private:
		struct SoundSource
		{
			Sound *TheSound;
			ALuint Source;
			SoundStatus Status;
			float CurrentGain;
			float CurrentPitch;
		};

	private:
		// See which sources that are playing
		void updateSources(float elapsed);
		// Refresh sources' data
		void refreshSource(SoundSource* source);

		glm::vec3 listenerPos, listenerVel;
		glm::vec3 listenerOriFor, listenerOriUp;

		uint maxSources;

		// Available sources (playing or not)
		std::vector<SoundSource> sources;
		std::vector<void(*)(void*, Sound*)> callbacks;

		float updateTimer;
		float updateInterval;
	};

}

#endif // KKE_SOUNDMANAGER_HPP
