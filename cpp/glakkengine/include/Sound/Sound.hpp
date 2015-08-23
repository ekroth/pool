/* Copyright (c) 2010-2011 Andrée Ekroth
 * See the file LICENSE.txt for copying permission.
 */

#ifndef KKE_SOUND_HPP
#define KKE_SOUND_HPP

#include "Interface/INoncopyable.hpp"
#include "Math.hpp"

#include <AL/alure.h>

namespace kke
{
	class SoundBuffer;
	class SoundManager;

	enum class SoundStatus
	{
		Unknown,
		Stopped,
		Playing,
		Paused,
		FadingOut,
		FadingIn
	};

	/**
	* @brief A sound.
	**/
	class Sound : public INoncopyable
	{
	public:
		explicit Sound(bool unique = false);
		~Sound();

		/**
		* @brief Get position.
		*
		* @return :Vector3f& Position.
		**/
		const glm::vec3& GetPosition() const;
		
		/**
		* @brief Set position.
		*
		* @param position Position.
		* @return void
		**/
		void SetPosition(const glm::vec3& position);

		/**
		* @brief Get velocity.
		*
		* @return :Vector3f& Velocity.
		**/
		const glm::vec3& GetVelocity() const;
		
		/**
		* @brief Set velocity.
		*
		* @param velocity Velocity.
		* @return void
		**/
		void SetVelocity(const glm::vec3& velocity);

		/**
		* @brief Get pitch.
		*
		* @return ALfloat Pitch.
		**/
		ALfloat GetPitch() const;
		
		/**
		* @brief Set pitch.
		*
		* @param value Pitch.
		* @return void
		**/
		void SetPitch(ALfloat value);

		/**
		* @brief Get gain.
		*
		* @return ALfloat Gain.
		**/
		ALfloat GetGain() const;
		
		/**
		* @brief Set gain.
		*
		* @param value Gain.
		* @return void
		**/
		void SetGain(ALfloat value);

		/**
		* @brief If Sound is looping.
		*
		* @return ALboolean Is looping.
		**/
		ALboolean IsLooping() const;
		
		/**
		* @brief Set looping.
		*
		* @param value Looping value.
		* @return void
		**/
		void SetLooping(ALboolean value);

		/**
		* @brief If sound is unique. Means there can only be one instance of this sound playing. This results in SoundManager::Pause/Stop working properly.
		*
		* @return bool
		**/
		bool IsUnique() const;
		
		/**
		* @brief If sound is unique. See bool IsUnique() for more information.
		*
		* @param value Unique.
		* @return void
		**/
		void SetUnique(bool value);

		/**
		* @brief Get status of sound. Unknown for non-unique Sounds.
		*
		* @return :SoundStatus
		**/
		SoundStatus GetStatus() const;
		
		/**
		* @brief Set Status of sound. Should normally be avoided.
		*
		* @param value Status.
		* @return void
		**/
		void SetStatus(SoundStatus value);

		/**
		* @brief Get used SoundBuffer.
		*
		* @return :SoundBuffer& SoundBuffer.
		**/
		const SoundBuffer& GetSoundBuffer() const;
		
		/**
		* @brief Set SoundBuffer to be used by Sound.
		*
		* @param soundBuffer SoundBuffer.
		* @return void
		**/
		void SetSoundBuffer(SoundBuffer* soundBuffer);

		/**
		* @brief Get fade speed.
		*
		* @return float Fade speed.
		**/
		float GetFadeSpeed() const;
		
		/**
		* @brief Set fade speed.
		*
		* @param value Fade speed.
		* @return void
		**/
		void SetFadeSpeed(float value);

	private:
		SoundBuffer *soundBuffer;
		glm::vec3 position;
		glm::vec3 velocity;
		ALfloat pitch;
		ALfloat gain;
		ALboolean loop;

		// Beings unique means that there can only be one instance
		// of this sound playing. This also results in Pause/Stop
		// working correctly.
		bool unique;
		SoundStatus status;

		float fadeSpeed;
	};
}

#endif // KKE_SOUND_HPP
