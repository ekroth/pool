/* Copyright (c) 2010-2011 Andrée Ekroth
 * See the file LICENSE.txt for copying permission.
 */

#ifndef KKE_SOUNDBUFFER_HPP
#define KKE_SOUNDBUFFER_HPP

#include "ResourceType.hpp"
#include "Interface/INoncopyable.hpp"

#include <AL/alure.h>

namespace kke
{
	/**
	* @brief SoundBuffer. Contains a audio data stream. Used by Sound.
	**/
	class SoundBuffer : public ResourceType, public INoncopyable
	{
	public:
		SoundBuffer();
		explicit SoundBuffer(const std::string& source);
		~SoundBuffer();

		/**
		* @brief Load a sound file.
		*
		* @param source Path to file.
		* @return bool Result.
		**/
		bool Load(const std::string& source);
		
		/**
		* @brief Unload SoundBuffer.
		*
		* @return bool Result.
		**/
		bool Unload();

		/**
		* @brief Get OpenAL buffer handle.
		*
		* @return ALuint OpenAL buffer handle.
		**/
		ALuint GetBuffer() const;

	private:
		ALuint buffer;
	};
}

#endif // KKE_SOUNDBUFFER_HPP
