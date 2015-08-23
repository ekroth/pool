/* Copyright (c) 2010-2011 Andrée Ekroth
 * See the file LICENSE.txt for copying permission.
 */

#include "Sound/SoundManager.hpp"
#include "Sound/SoundBuffer.hpp"
#include "Sound/Sound.hpp"
#include "IO/LogManager.hpp"
#include "IO/IniConfig.hpp"
#include "ResourceFactory.hpp"

using namespace kke;

using glm::vec2;
using glm::vec3;

using std::vector;
using std::string;

// Static
const vec3& SoundManager::GetListenerPos()
{
	return instance->listenerPos;
}

// Static
const vec3& SoundManager::GetListenerVel()
{
	return instance->listenerVel;
}

// Static
void SoundManager::SetListenerPos(const vec3& position)
{
	alListener3f(AL_POSITION, position.x, position.y, position.z);
	instance->listenerPos = position;
}

// Static
void SoundManager::SetListenerVel(const vec3& velocity)
{
	alListener3f(AL_VELOCITY, velocity.x, velocity.y, velocity.z);
	instance->listenerVel = velocity;
}

// Static
void SoundManager::MoveListenerPos(const vec3& delta)
{
	instance->listenerPos += delta;
	alListener3f(AL_POSITION, instance->listenerPos.x, instance->listenerPos.y, instance->listenerPos.z);
}

// Static
void SoundManager::MoveListenerVel(const vec3& delta)
{
	instance->listenerVel += delta;
	alListener3f(AL_VELOCITY, instance->listenerVel.x, instance->listenerVel.y, instance->listenerVel.z);
}

// Static
void SoundManager::Update(float elapsed)
{
	if (instance->updateTimer >= instance->updateInterval)
	{
		alureUpdate();
		// updateTimer is the elapsed time since last update
		instance->updateSources(instance->updateTimer);

		instance->updateTimer = 0.0f;

		if (!GetALErrors())
		{
			LOGE() << "Error at Update(float)";
		}
	}
	else
		instance->updateTimer += elapsed;
}

void SoundManager::Play(Sound* sound, bool fade)
{
	// First sound that was available
	vector<SoundSource>::iterator available = instance->sources.end();
	vector<SoundSource>::iterator uniqueAvailable = instance->sources.end();

	// TODO: Algorithm depends on if we want to use as many sources as possible (less switching),
	// 		 or use as few as possible (more switching).
	// Many sources: Unused > Unique > Stopped
	// Less sources: Unique > Stopped/Unused (* Using this *)
	for (auto it = instance->sources.begin(); it
		!= instance->sources.end() && uniqueAvailable == instance->sources.end(); ++it)
	{
		if (sound->IsUnique() && it->TheSound == sound)
		{
			// We found the same instance of the sound,
			// and since it is an "unique" Sound, it must
			// replace the other.
			available = it;
		}


		if (it->Status == SoundStatus::Stopped && available == instance->sources.end())
			available = it;
	}

	// Must use unique if found.
	if (uniqueAvailable != instance->sources.end())
		available = uniqueAvailable;

	if (available != instance->sources.end())
	{
		// Set source
		alSourceStop(available->Source);
		alSourcei(available->Source, AL_BUFFER, sound->GetSoundBuffer().GetBuffer());

		// Set Sound pointer
		available->TheSound = sound;
		available->CurrentGain = fade ? 0.0f : sound->GetGain();
		available->CurrentPitch = sound->GetPitch();

		instance->refreshSource(&*available);

		alSourcePlay(available->Source);
		alurePlaySource(available->Source, &CallbackStop, &(*available));
		available->Status = fade ? SoundStatus::FadingIn : SoundStatus::Playing;
		if (sound->IsUnique())
			sound->SetStatus(fade ? SoundStatus::FadingIn : SoundStatus::Playing);
		else
			sound->SetStatus(SoundStatus::Unknown);
	}
	else
	{
		// WARNING: Uhm, no source available. Will have to do
		// some prioritizing.
		// - Distance to sound
		// - Type of sound?
		LOGW() << "No more OpenAL sources available!";
	}

	if (!GetALErrors())
	{
		LOGE() << "Error at Play()";
	}
}

// Static
// TODO: Fade
void SoundManager::Stop(Sound* sound, bool fade)
{
	for (auto source : instance->sources)
	{
		if (source.TheSound == sound)
		{
			// Found source and sound
			alureStopSource(source.Source, AL_TRUE);
			source.Status = SoundStatus::Stopped;
			source.TheSound->SetStatus(source.TheSound->IsUnique() ? SoundStatus::Stopped : SoundStatus::Unknown);
		}
	}
}

// Static
void SoundManager::RemoveBuffer(SoundBuffer* buffer)
{
	// Stops all sources using this Buffer.
	for (auto source : instance->sources)
		if (source.TheSound != nullptr && source.TheSound->GetSoundBuffer().GetBuffer() == buffer->GetBuffer())
			Remove(source.TheSound);
}

void SoundManager::Remove(Sound* sound)
{
	for (auto source : instance->sources)
	{
		if (source.TheSound == sound)
		{
			// Found source and sound
			alureStopSource(source.Source, AL_FALSE);
			source.Status = SoundStatus::Stopped;
			source.TheSound = nullptr;
			alSourcei(source.Source, AL_BUFFER, 0); // Reset source's buffer
		}
	}
}

SoundManager::SoundManager()
{
	{
// 		string output = sysConfig->ReadString("AudioDevice");
		const char* outputDevice = /*output != "" ? output.c_str() :*/ 0;

		if (!alureInitDevice((const char*)outputDevice, 0))
		{
			LOGE() << "Error initializing alure.";
			GetALErrors();
			return;
		}
	}

	// Print information
	LOGM() << "Version: " << alGetString(AL_VERSION) << "\nRenderer " << alGetString(AL_RENDERER) <<
	"\nVendor" << alGetString(AL_VENDOR) << "\nExtensions: " <<	alGetString(AL_EXTENSIONS);

// 	maxSources = sysConfig->ReadInt("AudioSources", 32);
	updateInterval = 1.0 / 30.0f;
	updateTimer = 0.0f;

	// Reserve memory for maximum sources
	sources.reserve(maxSources);

	ALuint source[maxSources];
	alGenSources(maxSources, source);

	for (uint i = 0; i < maxSources; i++)
		sources.push_back(SoundSource{0, source[i], SoundStatus::Unknown, 0, 0});
	
	int deviceCount = 0;
	const char** deviceNames = alureGetDeviceNames(AL_TRUE, &deviceCount);

	LOGM() << "Devices available: ";
	for (int i = 0; i < deviceCount; i++)
	{
		LOGM() << deviceNames[i];
	}

	alureFreeDeviceNames(deviceNames);

	if (!GetALErrors())
	{
		LOGE() << "Error at SoundManager()";
	}
}

SoundManager::~SoundManager()
{
	if (!GetALErrors())
	{
		LOGE() << "Error before ~";
	}

	// WARNING: Make sure this works?
	for (auto source : instance->sources)
	{
		if (source.Status != SoundStatus::Stopped)
			alSourceStop(source.Source);

		alDeleteSources(1, &source.Source);
	}

	sources.clear();

	// Release OpenAL and alure resource
	if (alureShutdownDevice() == AL_FALSE)
	{
		LOGE() << "Error shutdown alure sound device.";
	}
}

void SoundManager::updateSources(float elapsed)
{
	for (auto source : instance->sources)
	{
		if (source.TheSound != nullptr)
		{
// 			{	*** Alure handles this now ***
// 				// Get state of sound
// 				ALint state;
// 				// Check if source is done playing
// 				alGetSourcei(it->Source, AL_SOURCE_STATE, &state);
// 
// 				// Have to poll if sound finished
// 				if (state == AL_STOPPED)
// 				{
// 					it->Status = SoundStopped;
// 					it->TheSound->SetStatus(it->TheSound->IsUnique() ? SoundStopped : SoundUnknown);
// 				}
// 			}

			if (source.Status == SoundStatus::FadingOut)
			{
				if (source.CurrentGain > 0.0f)
				{
					// Decrease gain while sound is louder than 0.0f
					source.CurrentGain -= source.TheSound->GetFadeSpeed() * elapsed;

					// When past or equal to 0, stop sound
					if (source.CurrentGain <= 0.0f)
					{
						Stop(source.TheSound);
					}
				}
			}
			else if (source.Status == SoundStatus::FadingIn)
				if (source.CurrentGain < source.TheSound->GetGain())
				{
					// Increase gain while gain is lower then
					// specified Sound gain.
					source.CurrentGain += source.TheSound->GetFadeSpeed() * elapsed;

					// When past or equal to Sound gain.
					if (source.CurrentGain >= source.TheSound->GetGain())
					{
						// Done
						source.Status = SoundStatus::Playing;
						source.TheSound->SetStatus(source.TheSound->IsUnique() ? SoundStatus::Playing : SoundStatus::Unknown);
						source.CurrentGain = source.TheSound->GetGain();
					}

					//Logger::Log("Sound gain: ");
					//Logger::Log(it->CurrentGain);
				}

			// Apply Sound values to OpenAL
			refreshSource(&source);
		}
	}
}

void SoundManager::refreshSource(SoundSource* source)
{
	// Set information
	alSource3f(source->Source, AL_POSITION, source->TheSound->GetPosition().x, source->TheSound->GetPosition().y, source->TheSound->GetPosition().z);
	alSource3f(source->Source, AL_VELOCITY, source->TheSound->GetVelocity().x, source->TheSound->GetVelocity().y, source->TheSound->GetVelocity().z);
	alSourcef(source->Source, AL_GAIN, source->CurrentGain);
	alSourcef(source->Source, AL_PITCH, source->CurrentPitch);
	alSourcei(source->Source, AL_LOOPING, source->TheSound->IsLooping() ? AL_TRUE : AL_FALSE);
}

// Static
bool SoundManager::GetALErrors()
{
	switch (alGetError())
	{
		case AL_NO_ERROR:
			return true;
			break;

		case AL_INVALID_NAME:
			LOGE() << "OpenAL, AL_INVALID_NAME.";
			break;

		case AL_INVALID_ENUM:
			LOGE() << "OpenAL, AL_INVALID_ENUM.";
			break;

		case AL_INVALID_VALUE:
			LOGE() << "OpenAL, AL_INVALID_VALUE.";
			break;

		case AL_INVALID_OPERATION:
			LOGE() << "OpenAL, AL_INVALID_OPERATION.";
			break;

		case AL_OUT_OF_MEMORY:
			LOGE() << "OpenAL, AL_OUT_OF_MEMORY.";
			break;

		default:
			LOGE() << "OpenAL, unknown error.";
			break;
	}

	return false;
}

// Callbacks for alure
void SoundManager::CallbackStop(void* userData, ALuint source)
{
  SoundSource *soundSource = (SoundSource*)userData;
  soundSource->Status = SoundStatus::Stopped;
  soundSource->TheSound->SetStatus(SoundStatus::Stopped);
//   soundSource->TheSound = nullptr;
}
