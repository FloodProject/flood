/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "vapor/PCH.h"

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
		Log::warn("Invalid audio device.");
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
	if(!context)
		return;
	
	if(device->context == context)
		device->context = nullptr;

	alcDestroyContext(context);
}

//-----------------------------------//

ALCcontext* AudioContext::createContext()
{
	ALCcontext* context = alcCreateContext(device->device, nullptr);

	if(checkError())
	{
		#pragma TODO("OpenAL getError() should be context-specific")
		Log::warn( getError().c_str() );
	}

	return context;
}

//-----------------------------------//

void AudioContext::setVolume(float volume)
{
	// Update OpenAL's volume.
	alListenerf(AL_GAIN, volume);

	if(checkError())
		Log::warn("Could not set new volume: %s", getError().c_str());
}

//-----------------------------------//

void AudioContext::setPosition(const Vector3& position)
{
	// Update OpenAL's listener position.
	alListenerfv(AL_POSITION, &position.x);

	if(checkError()) 
		Log::warn( "Could not set listener position: %s", getError().c_str());
}

//-----------------------------------//

void AudioContext::setOrientation(const Vector3& rotation)
{
	Vector3 data[] = { rotation, Vector3::UnitY };

	alListenerfv(AL_ORIENTATION, (const ALfloat *) &data );

	if(checkError()) 
		Log::warn( "Could not set listener orientation: %s", getError().c_str());
}

//-----------------------------------//

void AudioContext::makeCurrent()
{
	device->switchContext(context);
}

//-----------------------------------//

const std::string AudioContext::getError()
{
	const ALchar* str;
	
	switch (error)
	{
	case ALC_NO_ERROR:
		str = "No error."; 
		break;
    case ALC_INVALID_DEVICE:
        str = "Invalid device.";
        break;
    case ALC_INVALID_CONTEXT:
        str = "Invalid context.";
        break;
	case ALC_INVALID_ENUM:
		str = "Invalid enum.";
		break;
	case ALC_INVALID_VALUE:
		str = "Invalid value.";
		break;
    case ALC_OUT_OF_MEMORY:
        str = "Out of memory.";
        break;
	default:
		str = "Unknown error.";
		break;
	}
	
	return str;
}

//-----------------------------------//

bool AudioContext::checkError()
{
	error = alcGetError(device->device);
	return (error != AL_NO_ERROR);
}

//-----------------------------------//

} // end namespace

#endif