/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Engine/API.h"

#ifdef VAPOR_AUDIO_OPENAL

#include "Audio/Device.h"
#include "Audio/Buffer.h"
#include "Utilities.h"

namespace vapor {

//-----------------------------------//

static AudioDevice* gs_audioDevice = nullptr;
AudioDevice* GetAudioDevice() { return gs_audioDevice; }

//-----------------------------------//

AudioDevice::AudioDevice()
	: device(nullptr)
	, context(nullptr)
	, init(false)
	, error(AL_NO_ERROR)
	, mainContext(nullptr)
{
	if(!gs_audioDevice) gs_audioDevice = this;

	// Select the "preferred device".
	device = alcOpenDevice(nullptr);

	if( !device || checkError() )
	{
		LogWarn("Could not create OpenAL device: %s", getError());
		return;
	}

	LogInfo("Creating OpenAL main context");

	// Create a main context.
	mainContext = new AudioContext(this);

	const ALchar* version = alGetString(AL_VERSION);
	
	if( !version || checkError() )
	{
		LogWarn("Could not get OpenAL version");
		return;
	}
	
	LogInfo("Using OpenAL version %s", version);

	if( checkError() )
	{
		LogWarn("Error initializing OpenAL: %s", getError());
		return;
	}
}

//-----------------------------------//

AudioDevice::~AudioDevice()
{
	if(!device) return;

	#pragma TODO("Check that all contexts/buffers are closed on exit")
	delete mainContext;

	if(!alcCloseDevice(device)) 
	{
		LogWarn("Error closing OpenAL device: %s", getError());
		return;
	}
}

//-----------------------------------//

const std::string AudioDevice::getVersion() 
{
    ALCint major = 0;
	ALCint minor = 0;
    
	alcGetIntegerv(nullptr, ALC_MAJOR_VERSION, 1, &major);
    alcGetIntegerv(nullptr, ALC_MINOR_VERSION, 1, &minor);
    
	return StringFormat("%d.%d", major, minor);
}

//-----------------------------------//

void AudioDevice::setVolume(float volume)
{
	alListenerf(AL_GAIN, volume);

	if(checkError()) 
	{
		LogWarn("Error changing listener volume: %s", getError());
		return;
	}
}

//-----------------------------------//

ALint AudioDevice::getFormat(const SoundPtr& sound)
{
	if( !sound ) return AL_INVALID;

	int channels = sound->getChannels();
	int size = sound->getSize();

	if(channels == 1 && size == 8) return AL_FORMAT_MONO8;
	else if(channels == 1 && size == 16) return AL_FORMAT_MONO16;
	else if(channels == 2 && size ==  8) return AL_FORMAT_STEREO8;
	else if(channels == 2 && size == 16) return AL_FORMAT_STEREO16;

	return AL_INVALID;
}

//-----------------------------------//

AudioBufferPtr AudioDevice::prepareBuffer(const SoundPtr& sound)
{
	// Check if buffer with same resource already exists
	if( soundBuffers.find(sound) != soundBuffers.end() ) 
		return soundBuffers[sound];

	AudioBufferPtr buffer( new AudioBuffer(this, sound) );
	soundBuffers[sound] = buffer;

	return buffer;
}

//-----------------------------------//

bool AudioDevice::checkError()
{
	error = alGetError();
	return (error != AL_NO_ERROR);
}

//-----------------------------------//

const ALchar* AudioDevice::getError()
{
	switch(error)
	{
	case AL_NO_ERROR: return "No error";
	case AL_INVALID_ENUM: return "Invalid enum";
	case AL_INVALID_VALUE: return "Invalid value";
	case AL_INVALID_NAME: return "Invalid name";
	default: return "Unknown error";
	}
}

//-----------------------------------//

} // end namespace

#endif