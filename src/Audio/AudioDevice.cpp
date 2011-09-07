/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Engine/API.h"

#ifdef ENABLE_AUDIO_OPENAL

#include "Audio/Device.h"
#include "Audio/Buffer.h"
#include "Audio/Context.h"
#include "Audio/AL.h"
#include <alext.h>

NAMESPACE_ENGINE_BEGIN

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

bool AudioDevice::getExtensions(std::vector<String>& extensions)
{
	const ALCchar* allExtensions = alcGetString(nullptr, ALC_EXTENSIONS);

	if( !allExtensions )
		return false;

	StringSplit(allExtensions, ' ', extensions);

	return true;
}

//-----------------------------------//

bool AudioDevice::getDevices(std::vector<String>& devices)
{
	if (!alcIsExtensionPresent(nullptr, "ALC_ENUMERATE_ALL_EXT"))
		return false;

	const ALCchar* allDevices = alcGetString(nullptr, ALC_ALL_DEVICES_SPECIFIER);
	StringSplit(allDevices, ' ', devices);

	return true;
}

//-----------------------------------//

bool AudioDevice::createDevice(const String& deviceName)
{
	if( device != nullptr )
		return false;

	const char* name = deviceName.empty() ? nullptr : deviceName.c_str();

	// Select the "preferred device".
	device = alcOpenDevice(name);

	if( !device )
	{
		LogWarn("Could not create OpenAL device");
		return false;
	}

	return true;
}

//-----------------------------------//

bool AudioDevice::createMainContext()
{
	if( device == nullptr )
		return false;

	LogInfo("Creating OpenAL main context");

	// Create a main context.
	mainContext = new AudioContext(this);

	const ALchar* version = alGetString(AL_VERSION);
	
	if( !version )
	{
		LogWarn("Could not get OpenAL version");
		return false;
	}
	
	LogInfo("Using OpenAL version %s", version);

	if( checkError() )
	{
		LogWarn("Error initializing OpenAL: %s", getError());
		return false;
	}

	return true;
}

//-----------------------------------//

const String AudioDevice::getVersion() 
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

ALint AudioDevice::getFormat(Sound* sound)
{
	if( !sound ) return AL_INVALID;

	int32 channels = sound->getChannels();
	int32 size = sound->getSize();

	if(channels == 1 && size == 8) return AL_FORMAT_MONO8;
	else if(channels == 1 && size == 16) return AL_FORMAT_MONO16;
	else if(channels == 2 && size ==  8) return AL_FORMAT_STEREO8;
	else if(channels == 2 && size == 16) return AL_FORMAT_STEREO16;

	return AL_INVALID;
}

//-----------------------------------//

AudioBufferPtr AudioDevice::prepareBuffer(Sound* sound)
{
	// Check if buffer with same resource already exists
	if( soundBuffers.find(sound) != soundBuffers.end() ) 
		return soundBuffers[sound];

	AudioBufferPtr buffer = Allocate(AudioBuffer, AllocatorGetHeap(), this, sound);
	soundBuffers[sound] = buffer;

	return buffer;
}

//-----------------------------------//

bool AudioDevice::checkError()
{
#ifdef BUILD_DEBUG
	error = alGetError();
	return (error != AL_NO_ERROR);
#else
	return false;
#endif
}

//-----------------------------------//

#ifdef BUILD_DEBUG
const ALchar* AudioDevice::getError()
{
	switch(error)
	{
	case AL_NO_ERROR:
		return "No error";
	case AL_INVALID_ENUM:
		return "Invalid enum";
	case AL_INVALID_VALUE:
		return "Invalid value";
	case AL_INVALID_NAME:
		return "Invalid name";
	default:
		return "Unknown error";
	}
}
#endif

//-----------------------------------//

NAMESPACE_ENGINE_END

#endif