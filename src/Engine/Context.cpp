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

#include "vapor/audio/Context.h"

using namespace vapor::math;

namespace vapor {
	namespace audio {

//-----------------------------------//

Context::Context(audio::Device* device)
	: context(nullptr), error(ALC_NO_ERROR), device(device)
{
	if(!device)
	{
		warn("audio::al", "Could not create a new context \
						  because of an invalid device pointer.");
		return;
	}

	context = createContext();

	// set a default listener
	setListener(Vector3(0.0f, 0.0f, 0.0f));

	// set default volume
	setVolume(1.0f);
}

//-----------------------------------//

Context::~Context()
{
	if(context) 
	{
		if(device->ctx == context)
			device->ctx = nullptr;

		alcDestroyContext(context);
	}
}

//-----------------------------------//

ALCcontext* Context::createContext()
{
	ALCcontext* context = nullptr;
	
	context = alcCreateContext(device->device, nullptr);

	if(checkError())
	{
		// TODO: getError should be for context-specific
		warn("audio::al", "Could not create OpenAL context: %s",
			getError());
	}

	return context;
}

//-----------------------------------//

void Context::setVolume(float volume)
{
	// update OpenAL volume (or gain)
	alListenerf(AL_GAIN, volume);

	if(checkError())
	{
		warn("audio::al", "Could not set a new volume: %s",
			getError().c_str());
	}
}

//-----------------------------------//

void Context::setListener(const Vector3& position)
{
	// update OpenAL listener position
	alListener3f(AL_POSITION, position.x, position.y, position.z);

	if(checkError()) 
	{
		warn("audio::al", "Error changing listener position: %s",
			getError().c_str());
	}
}

//-----------------------------------//

void Context::makeCurrent()
{
	device->switchContext(this->context);
}

//-----------------------------------//

const string Context::getError()
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

bool Context::checkError()
{
	error = alcGetError(device->device);
	return (error != AL_NO_ERROR);
}

//-----------------------------------//

} } // end namespaces

#endif