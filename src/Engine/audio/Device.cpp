/************************************************************************
*
* vaporEngine (2008-2009)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#include "vapor/audio/Device.h"

//#ifdef VAPOR_AUDIO_OPENAL

using namespace vapor::resources;
using namespace vapor::math;

namespace vapor {
	namespace audio {

//-----------------------------------//

AudioDevice::AudioDevice()
{
	// select the "preferred device"
	device = alcOpenDevice(nullptr); 
	
	if(!device)  
	{
		warn("audio::al", "Could not create OpenAL device");
	}

	info("audio::al", "Using OpenAL %s", alGetString(AL_VERSION));

	if(alGetError() != AL_NO_ERROR) {
		warn("audio::al", "Error initializing OpenAL");
	}

	// set a default listener
	setListener(Vector3(0.0f, 0.0f, 0.0f));

	// set default volume
	setVolume(1.0f);
}

//-----------------------------------//

AudioDevice::~AudioDevice()
{

}

//-----------------------------------//

void AL::switchListener(scene::Listener* listener)
{
	listenerContexts[listener] = alcCreateContext(device, nullptr);
	alcMakeContextCurrent(listenerContexts[listener]);
}

//-----------------------------------//

void AudioDevice::setListener(const Vector3 position)
{
	// update OpenAL position information
	alListener3f(AL_POSITION, position.x, position.y, position.z);

	if(alGetError() != AL_NO_ERROR) {
		warn("audio::al", "Error changing listener position");
	}
}

//-----------------------------------//

void AudioDevice::setVolume(float volume)
{
	alListenerf(AL_GAIN, volume);

	if(alGetError() != AL_NO_ERROR) {
		warn("audio::al", "Error changing listener volume");
	}
}

//-----------------------------------//

ALint AudioDevice::getALFormat(SoundFormat::Enum format)
{
	switch(format)
	{
	case SoundFormat::MONO8:
		return AL_FORMAT_MONO8;
	case SoundFormat::MONO16:
		return AL_FORMAT_MONO16;
	case SoundFormat::STEREO8:
		return AL_FORMAT_STEREO8;
	case SoundFormat::STEREO16:
		return AL_FORMAT_STEREO16;
	default:
		return AL_INVALID;
	}
}

//-----------------------------------//

void AudioDevice::play2D(const Sound *sound, bool loop)
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

ALuint AudioDevice::prepareBuffer(const Sound* sound)
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

//#endif