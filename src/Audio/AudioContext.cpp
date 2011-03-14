/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Engine/API.h"

#ifdef VAPOR_AUDIO_OPENAL

#include "Audio/Context.h"
#include "Audio/Device.h"
#include "Audio/AL.h"

namespace vapor {

//-----------------------------------//

AudioContext::AudioContext(AudioDevice* device)
	: device(device)
	, context(nullptr)
	, error(ALC_NO_ERROR)
{
	if(!device)
	{
		LogWarn("Invalid audio device.");
		return;
	}

	context = createContext();
	makeCurrent();

	// Sets a default listener.
	setPosition(Vector3::Zero);

	// Sets the default volume.
	setVolume(1.0f);
}

//-----------------------------------//

AudioContext::~AudioContext()
{
	if(!context) return;

	makeCurrent();

	alcDestroyContext(context);
	alcMakeContextCurrent(nullptr);

	if(device->context == context)
		device->context = nullptr;
}

//-----------------------------------//

ALCcontext* AudioContext::createContext()
{
	ALCcontext* context = alcCreateContext(device->device, nullptr);

	if(checkError())
	{
		LogWarn("Error creating OpenAL context", getError());
		return nullptr;
	}

	return context;
}

//-----------------------------------//

void AudioContext::setVolume(float volume)
{
	// Update OpenAL's volume.
	alListenerf(AL_GAIN, volume);

	if(checkError())
	{
		LogWarn("Could not set new volume: %s", getError());
		return;
	}
}

//-----------------------------------//

void AudioContext::setPosition(const Vector3& position)
{
	// Update OpenAL's listener position.
	alListenerfv(AL_POSITION, &position.x);

	if(checkError())
	{
		LogWarn( "Could not set listener position: %s", getError());
		return;
	}
}

//-----------------------------------//

void AudioContext::setOrientation(const Vector3& rotation)
{
	Vector3 data[] = { rotation, Vector3::UnitY };

	alListenerfv(AL_ORIENTATION, (const ALfloat *) &data );

	if(checkError())
	{
		LogWarn( "Could not set listener orientation: %s", getError());
		return;
	}
}

//-----------------------------------//

void AudioContext::makeCurrent()
{
	ALCboolean ret = alcMakeContextCurrent(context);

	if(ret != ALC_TRUE)
	{
		LogWarn("Could not make OpenAL context current");
		return;
	}
}

//-----------------------------------//

const char* AudioContext::getError()
{
#ifdef VAPOR_DEBUG
	switch (error)
	{
	case ALC_NO_ERROR: return "No error";
    case ALC_INVALID_DEVICE: return "Invalid device";
    case ALC_INVALID_CONTEXT: return "Invalid context";
	case ALC_INVALID_ENUM: return "Invalid enum";
	case ALC_INVALID_VALUE: return "Invalid value";
    case ALC_OUT_OF_MEMORY: return "Out of memory";
	default: return "Unknown error";
	}
#else
	return nullptr;
#endif
}

//-----------------------------------//

bool AudioContext::checkError()
{
#ifdef VAPOR_DEBUG
	error = alcGetError(device->device);
	return (error != AL_NO_ERROR);
#else
	return false;
#endif
}

//-----------------------------------//

} // end namespace

#endif