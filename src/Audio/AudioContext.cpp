/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Engine/API.h"

#ifdef ENABLE_AUDIO_OPENAL

#include "Audio/Context.h"
#include "Audio/Device.h"
#include "Audio/Source.h"
#include "Audio/AL.h"

NAMESPACE_ENGINE_BEGIN

//-----------------------------------//

AudioContext::AudioContext(ALCcontext* context)
	: context(context)
	, error(ALC_NO_ERROR)
{
	// Sets a default listener.
	setPosition(Vector3::Zero);

	// Sets the default volume.
	setVolume(1.0f);
}

//-----------------------------------//

AudioContext::~AudioContext()
{
	if(!context) return;

	alcDestroyContext(context);
	alcMakeContextCurrent(nullptr);
}

//-----------------------------------//

AudioSourcePtr AudioContext::createSource()
{
	return AllocateThis(AudioSource, this);
}

//-----------------------------------//

void AudioContext::setVolume(float volume)
{
	// Update OpenAL's volume.
	alListenerf(AL_GAIN, volume);

	if(AudioCheckError())
	{
		LogWarn("Could not set new volume: %s", AudioGetError());
		return;
	}
}

//-----------------------------------//

void AudioContext::setPosition(const Vector3& position)
{
	// Update OpenAL's listener position.
	alListenerfv(AL_POSITION, &position.x);

	if(AudioCheckError())
	{
		LogWarn( "Could not set listener position: %s", AudioGetError());
		return;
	}
}

//-----------------------------------//

void AudioContext::setOrientation(const Vector3& rotation)
{
	Vector3 data[] = { rotation, Vector3::UnitY };

	alListenerfv(AL_ORIENTATION, (const ALfloat *) &data );

	if(AudioCheckError())
	{
		LogWarn( "Could not set listener orientation: %s", AudioGetError());
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

void AudioContext::process()
{
	alcProcessContext(context);
}

//-----------------------------------//

NAMESPACE_ENGINE_END

#endif