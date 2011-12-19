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
#include "Resources/ResourceManager.h"

NAMESPACE_ENGINE_BEGIN

//-----------------------------------//

static AudioDevice* gs_audioDevice = nullptr;
AudioDevice* GetAudioDevice() { return gs_audioDevice; }

//-----------------------------------//

AudioDevice::AudioDevice(ALCdevice* device)
	: device(device)
	, mainContext(nullptr)
{
	if(!gs_audioDevice) gs_audioDevice = this;

	ResourceManager* res = GetResourceManager();
	res->onResourceLoaded.Connect(this, &AudioDevice::onResourceLoaded);
}

//-----------------------------------//

AudioDevice::~AudioDevice()
{
	if(!device) return;

	#pragma TODO("Check that all contexts/buffers are closed on exit")
	Deallocate(mainContext);

	if(!alcCloseDevice(device)) 
	{
		LogWarn("Error closing OpenAL device: %s", AudioGetError());
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

bool AudioDevice::createMainContext()
{
	LogInfo("Creating OpenAL main context");

	// Create a main context.
	mainContext = createContext();

	mainContext->makeCurrent();

	const ALchar* version = alGetString(AL_VERSION);
	
	if( !version )
	{
		LogWarn("Could not get OpenAL version");
		return false;
	}
	
	LogInfo("Using OpenAL version %s", version);

	if( AudioCheckError() )
	{
		LogWarn("Error initializing OpenAL: %s", AudioGetError());
		return false;
	}

	return true;
}

//-----------------------------------//

AudioContext* AudioDevice::createContext()
{
	ALCcontext* context = alcCreateContext(device, nullptr);

	if( !context || alcGetError(device) )
	{
		LogWarn("Error creating OpenAL context");
		return nullptr;
	}

	AudioContext* audioContext = AllocateThis(AudioContext, context);

	return audioContext;
}

//-----------------------------------//

const String AudioGetVersion() 
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

	if(AudioCheckError()) 
	{
		LogWarn("Error changing listener volume: %s", AudioGetError());
		return;
	}
}

//-----------------------------------//

AudioBufferPtr AudioDevice::createBuffer()
{
	return AllocateThis(AudioBuffer, this);
}

//-----------------------------------//

AudioBufferPtr AudioDevice::prepareBuffer(Sound* sound)
{
	// Check if buffer with same resource already exists
	if( soundBuffers.find(sound) != soundBuffers.end() ) 
		return soundBuffers[sound];

	AudioBufferPtr buffer = AllocateThis(AudioBuffer, this);
	soundBuffers[sound] = buffer;

	return buffer;
}

//-----------------------------------//

void AudioDevice::onResourceLoaded(const ResourceEvent& event)
{
	Resource* resource = event.resource;
	assert( resource != nullptr );

	if( resource->getResourceGroup() != ResourceGroup::Audio )
		return;

	if( !resource->inherits<Sound>() )
		return;

	Sound* sound = (Sound*) resource;
	assert( sound->isLoaded() );

	auto it = soundBuffers.find(sound);
	
	if( it == soundBuffers.end() )
		return;

	AudioBuffer* soundBuffer = it->second.get();
	AudioBufferSound(soundBuffer, sound);
}

//-----------------------------------//

bool AudioGetDevices(std::vector<String>& devices)
{
	if (!alcIsExtensionPresent(nullptr, "ALC_ENUMERATE_ALL_EXT"))
		return false;

	const ALCchar* allDevices = alcGetString(nullptr, ALC_ALL_DEVICES_SPECIFIER);
	StringSplit(allDevices, ' ', devices);

	return true;
}

//-----------------------------------//

ALint AudioGetFormat(Sound* sound)
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

AudioDevice* AudioCreateDevice(const String& deviceName)
{
	const char* name =
		deviceName.empty() ? nullptr : deviceName.c_str();

	// Select the "preferred device".
	ALCdevice* device = alcOpenDevice(name);

	if( !device )
	{
		LogWarn("Could not create OpenAL device");
		return nullptr;
	}

	AudioDevice* audioDevice = AllocateHeap(AudioDevice, device);
	
	return audioDevice;
}

//-----------------------------------//

#ifdef BUILD_DEBUG
thread_local ALenum  gs_AudioError = AL_NO_ERROR;
#endif

bool AudioCheckError()
{
#ifdef BUILD_DEBUG
	gs_AudioError = alGetError();
	return (gs_AudioError != AL_NO_ERROR);
#else
	return false;
#endif
}

//-----------------------------------//

#ifdef BUILD_DEBUG
const ALchar* AudioGetError()
{
	switch(gs_AudioError)
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