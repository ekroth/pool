/* Copyright (c) 2010-2011 Andrée Ekroth
 * See the file LICENSE.txt for copying permission.
 */

// TODO: Use libsndfile

#include "Sound/SoundBuffer.hpp"
#include "Sound/SoundManager.hpp"
#include "IO/LogManager.hpp"

using namespace kke;

using std::string;

SoundBuffer::SoundBuffer() :
	ResourceType(), buffer(0)
{
}

SoundBuffer::~SoundBuffer()
{
	Unload();
}

bool SoundBuffer::Load(const string& source)
{
	this->source = source;
	Unload();

	// Create a buffer from the source
	//buffer = alureCreateBufferFromFile(source.c_str());
	// FIXME: Load correctly.
	//buffer = alutCreateBufferHelloWorld();


	buffer = alureCreateBufferFromFile(source.c_str());

	if (buffer == AL_NONE)
	{
		LOGE() << "Error loading SoundBuffer (" << source << ')';
		return false;
	}

	this->source = source;

	return true;
}

bool SoundBuffer::Unload()
{
	// Must notify SoundManager, to remove all Sounds using this buffer.
	SoundManager::Instance().RemoveBuffer(this);

	if (buffer != 0)
	{
		// Delete existing buffer
		alDeleteBuffers(1, &buffer);
		buffer = 0;
	}

	return buffer == 0;
}

ALuint SoundBuffer::GetBuffer() const
{
	return buffer;
}
