/* Copyright (c) 2010-2011 Andrée Ekroth
 * See the file LICENSE.txt for copying permission.
 */

#include "Sound/Sound.hpp"
#include "Sound/SoundManager.hpp"

using namespace kke;

using glm::vec3;
using glm::vec2;

Sound::Sound(bool unique) :
	soundBuffer(nullptr), position(0, 0, 0), velocity(0, 0, 0), pitch(1.0f),
		gain(1.0f), loop(0), unique(unique),
		status(unique ? SoundStatus::Stopped : SoundStatus::Unknown), fadeSpeed(0.75f)
{
}

Sound::~Sound()
{
	// Must notify SoundManager
	SoundManager::Instance().Remove(this);
}

const vec3& Sound::GetPosition() const
{
	return position;
}

void Sound::SetPosition(const vec3& position)
{
	this->position = position;
}

const vec3& Sound::GetVelocity() const
{
	return velocity;
}

void Sound::SetVelocity(const vec3 &velocity)
{
	this->velocity = velocity;
}

ALfloat Sound::GetPitch() const
{
	return pitch;
}

void Sound::SetPitch(ALfloat value)
{
	pitch = value;
}

ALfloat Sound::GetGain() const
{
	return gain;
}

void Sound::SetGain(ALfloat value)
{
	gain = value;
}

ALboolean Sound::IsLooping() const
{
	return loop;
}

void Sound::SetLooping(ALboolean value)
{
	loop = value;
}

bool Sound::IsUnique() const
{
	return unique;
}

void Sound::SetUnique(bool value)
{
	unique = value;
}

SoundStatus Sound::GetStatus() const
{
	return status;
}

void Sound::SetStatus(SoundStatus value)
{
	// Only if the Sound is unique, otherwise
	// this value is very unreliable.
	status = unique ? value : SoundStatus::Unknown;
}

const SoundBuffer& Sound::GetSoundBuffer() const
{
	return *soundBuffer;
}

void Sound::SetSoundBuffer(SoundBuffer* soundBuffer)
{
	this->soundBuffer = soundBuffer;
}

float Sound::GetFadeSpeed() const
{
	return fadeSpeed;
}
void Sound::SetFadeSpeed(float value)
{
	fadeSpeed = value;
}
