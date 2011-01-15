/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "vapor/PCH.h"

#ifdef VAPOR_AUDIO_OPENAL

#include "audio/Device.h"
#include "audio/Buffer.h"
#include "Utilities.h"

#pragma TODO("Init first context to log version")

namespace vapor {

//-----------------------------------//

AudioDevice::AudioDevice()
	: device(nullptr)
	, context(nullptr)
	, init(false)
	, error(AL_NO_ERROR)
	, mainContext(nullptr)
{
	// Select the "preferred device".
	device = alcOpenDevice(nullptr);

	// Create a main context.
	mainContext = new AudioContext(this);
	
	if( !device || checkError() )
	{
		Log::warn("Could not create OpenAL device: %s", getError());
		return;
	}

	const ALchar* version = alGetString(AL_VERSION);
	
	if( !version || checkError() )
	{
		Log::warn("Could not get OpenAL version");
		return;
	}
	
	Log::info("Using OpenAL version %s", version);

	if( checkError() )
	{
		Log::warn("Error initializing OpenAL: %s", getError());
		return;
	}
}

//-----------------------------------//

AudioDevice::~AudioDevice()
{
	if(!device) return;

	#pragma TODO("Check that all contexts/buffers are closed on exit")
	
	ALCboolean ret = alcCloseDevice(device);
		
	if(ret != ALC_TRUE) 
	{
		Log::warn("Error closing OpenAL device: %s", getError());
		return;
	}

	device = nullptr;
}

//-----------------------------------//

const std::string AudioDevice::getVersion() 
{
    ALCint major = 0;
	ALCint minor = 0;
    
	alcGetIntegerv(nullptr, ALC_MAJOR_VERSION, 1, &major);
    alcGetIntegerv(nullptr, ALC_MINOR_VERSION, 1, &minor);
    
	return String::format("%d.%d", major, minor);
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
	const ALchar* str;
	
	switch(error)
	{
	case AL_NO_ERROR:
		str = "No error."; 
		break;
	case AL_INVALID_ENUM:
		str = "Invalid enum.";
		break;
	case AL_INVALID_VALUE:
		str = "Invalid value.";
		break;
	case AL_INVALID_NAME:
		str = "Invalid name.";
		break;
	default:
		str = "Unknown error.";
		break;
	}
	
	return str;
}

//-----------------------------------//

void AudioDevice::setVolume(float volume)
{
	alListenerf(AL_GAIN, volume);

	if(checkError()) 
	{
		Log::warn("Error changing listener volume: %s", getError());
	}
}

//-----------------------------------//

ALint AudioDevice::getFormat(const SoundPtr& sound)
{
	if( !sound ) return AL_INVALID;

	int channels = sound->getChannels();
	int size = sound->getSize();

	if(channels == 1 && size == 8)
		return AL_FORMAT_MONO8;

	if(channels == 1 && size == 16)
		return AL_FORMAT_MONO16;

	if(channels == 2 && size == 8)
		return AL_FORMAT_STEREO8;

	if(channels == 2 && size == 16)
		return AL_FORMAT_STEREO16;

	return AL_INVALID;
}

//-----------------------------------//

void AudioDevice::switchContext(ALCcontext* context)
{
	ALCboolean ret = alcMakeContextCurrent(context);

	if(ret != ALC_TRUE)
	{
		Log::warn("Could not make OpenAL context current");
		return;
	}

	this->context = context;
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

} // end namespace

#endif