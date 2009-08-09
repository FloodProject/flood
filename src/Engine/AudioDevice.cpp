/************************************************************************
*
* vaporEngine (2008-2009)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#include "vapor/CompileOptions.h"

#ifdef VAPOR_AUDIO_OPENAL

#include <sstream>

#include "vapor/audio/Device.h"
#include "vapor/audio/Buffer.h"

using namespace vapor::resources;
using namespace vapor::math;

namespace vapor {
	namespace audio {

// TODO: init first context to log version

//-----------------------------------//

Device::Device()
	: device(nullptr), ctx(nullptr), init(false), error(AL_NO_ERROR)
{
	// select the "preferred device"
	device = alcOpenDevice(nullptr); 
	
	if( !device || checkError() )
	{
		warn("audio::al", "Could not create OpenAL device: %s", getError());
	}

	const ALchar* version = alGetString(AL_VERSION);
	
	if( !version || checkError() ) 
	{
		warn("audio::al", "Could not get OpenAL version");
	} else {
		info("audio::al", "Using OpenAL version %s", version);
	}

	if( checkError() )
	{
		warn("audio::al", "Error initializing OpenAL: %s", getError());
	}
}

//-----------------------------------//

Device::~Device()
{
	if(!device) return;

	// TODO: check that all contexts/buffers are closed
	
	ALCboolean ret = alcCloseDevice(device);
		
	if(ret != ALC_TRUE) 
	{
		warn("audio::al", "Error closing OpenAL device: %s", getError());
		return;
	}

	device = nullptr;
}

//-----------------------------------//

const std::string Device::getVersion() 
{
    std::stringstream s;
    ALCint major = 0, minor = 0;
    
	alcGetIntegerv(nullptr, ALC_MAJOR_VERSION, 1, &major);
    alcGetIntegerv(nullptr, ALC_MINOR_VERSION, 1, &minor);
    s << major << "." << minor;
    
	return s.str();
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

void Device::setVolume(float volume)
{
	alListenerf(AL_GAIN, volume);

	if(checkError()) 
	{
		warn("audio::al", "Error changing listener volume: %s", getError());
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

void Device::switchContext(ALCcontext* context)
{
	// if context is already current, return.
	if(ctx && (ctx == context))
		return;

	ALCboolean ret = alcMakeContextCurrent(context);

	if( (ret != ALC_TRUE))
	{
		warn("audio::al", "Could not make OpenAL context current");
		return;
	}

	this->ctx = context;
}

//-----------------------------------//

shared_ptr<Buffer> Device::prepareBuffer(shared_ptr<resources::Sound> sound)
{
	// check if buffer with same resource already exists
	if(soundBuffers.find(sound) != soundBuffers.end()) 
		return soundBuffers[sound];

	shared_ptr<Buffer> buf(new Buffer(this, sound));
	soundBuffers[sound] = buf;

	return buf;
}

//-----------------------------------//

} } // end namespaces

#endif