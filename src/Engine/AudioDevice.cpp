/************************************************************************
*
* vaporEngine (2008-2009)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#include "vapor/CompileOptions.h"

#ifdef VAPOR_AUDIO_OPENAL

#include <al.h>
#include <alc.h>

#include "vapor/audio/Device.h"

using namespace vapor::resources;
using namespace vapor::math;

namespace vapor {
	namespace audio {

//-----------------------------------//

Device::Device()
	: device(nullptr), ctx(nullptr), init(false)
{
	// select the "preferred device"
	device = alcOpenDevice(nullptr); 
	
	if(!device || checkError()) {
		warn("audio::al", "Could not create OpenAL device: %s", getError());
	}

	info("audio::al", "Using OpenAL %s", alGetString(AL_VERSION));

	// set a default listener
	setListener(Vector3(0.0f, 0.0f, 0.0f));

	if(checkError()) {
		warn("audio::al", "Error initializing OpenAL: %s", getError());
	}

	// set default volume
	setVolume(1.0f);
}

//-----------------------------------//

Device::~Device()
{
	if(!device) return;

	// TODO: check that all contexts/buffers are closed
	
	ALCboolean ret = alcCloseDevice(device);
		
	if(ret == ALC_FALSE) {
		warn("audio::al", "Error closing OpenAL device");
		return;
	}
	
	device = nullptr;
}

//-----------------------------------//

bool Device::checkError()
{
	error = alGetError();
	return (error != AL_NO_ERROR);
}

//-----------------------------------//

const ALchar* Device::getError()
{
	const ALchar* str;
	
	switch (error)
	{
	case AL_NO_ERROR:
		str = "No error."; 
		break;
	case AL_INVALID_NAME:
		str = "Invalid name.";
		break;
	case AL_INVALID_ENUM:
		str = "Invalid enum.";
		break;
	case AL_INVALID_VALUE:
		str = "Invalid value.";
		break;
	default:
		str = "Unknown error.";
		break;
	}
	
	return str;
}

//-----------------------------------//

void Device::switchListener(scene::Listener* listener)
{
	listenerContexts[listener] = alcCreateContext(device, nullptr);
	alcMakeContextCurrent(listenerContexts[listener]);
}

//-----------------------------------//

void Device::setListener(const Vector3& position)
{
	// update OpenAL position information
	alListener3f(AL_POSITION, position.x, position.y, position.z);

	if(alGetError() != AL_NO_ERROR) {
		warn("audio::al", "Error changing listener position");
	}
}

//-----------------------------------//

void Device::setVolume(float volume)
{
	alListenerf(AL_GAIN, volume);

	if(alGetError() != AL_NO_ERROR) {
		warn("audio::al", "Error changing listener volume");
	}
}

//-----------------------------------//

ALint Device::getALFormat(SoundFormat::Enum format)
{
	switch(format)
	{
	case SoundFormat::Mono8:
		return AL_FORMAT_MONO8;
	case SoundFormat::Mono16:
		return AL_FORMAT_MONO16;
	case SoundFormat::Stereo8:
		return AL_FORMAT_STEREO8;
	case SoundFormat::Stereo16:
		return AL_FORMAT_STEREO16;
	default:
		return AL_INVALID;
	}
}

//-----------------------------------//

void Device::play2D(const Sound *sound, bool loop)
{
	ALuint buffer = prepareBuffer(sound);

	ALuint sourceID;

	// Generate Buffers
	alGetError(); // clear error code
	alGenSources(1, &sourceID);

	// Set the source and listener to the same location
	alSource3f(sourceID, AL_POSITION, 0.0f, 0.0f, 0.0f);
}

//-----------------------------------//

ALuint Device::prepareBuffer(const Sound* sound)
{
	// check if buffer with same sound already exists
	if(soundBuffers.find(sound) != soundBuffers.end()) 
		return soundBuffers[sound];

	// if not, create a new buffer and upload sound data
	ALuint bufferID;
	alGenBuffers(1, &bufferID);

	// check if buffer was successfuly created
	if(alGetError() != AL_NO_ERROR) {
		warn("audio::al", "Error creating a sound buffer");
	}

	// update buffer id in the map
	soundBuffers[sound] = bufferID;

	// upload sound data to buffer
	alBufferData(bufferID, getALFormat(sound->getFormat()), &sound->getBuffer()[0], 
		static_cast <ALsizei> (sound->getBuffer().size()), sound->getFrequency());
	
	if(alGetError() != AL_NO_ERROR) {
		warn("audio::al", "Error uploading sound to buffer");
	}

	return bufferID;
}

//-----------------------------------//

} } // end namespaces

#endif