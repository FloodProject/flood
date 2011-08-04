/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Engine/API.h"

#ifdef ENABLE_AUDIO_OPENAL

#include "Audio/Source.h"
#include "Audio/Buffer.h"

NAMESPACE_ENGINE_BEGIN

//-----------------------------------//

REFLECT_ENUM(RolloffMode)
	ENUM(Logarithmic)
	ENUM(Linear)
	ENUM(Exponential)
REFLECT_ENUM_END()

//-----------------------------------//

AudioSource::AudioSource(AudioContext* context, const SoundHandle& handle)
	: context(context)
	, device(context->device)
	, id(0)
{	
	context->makeCurrent();

	// Generates a new OpenAL source.
	alGenSources(1, &id);

	if(device->checkError())
	{
		LogWarn("Could not generate a new audio source: %s", device->getError());
		return;
	}

	Sound* sound = handle.Resolve();
	buffer = device->prepareBuffer(sound);
	
	setPosition( Vector3::Zero );
}

//-----------------------------------//

AudioSource::~AudioSource()
{
	context->makeCurrent();

	stop();

	alSourcei(id, AL_BUFFER, AL_NONE);

	if( device->checkError() )
	{
		LogWarn("Could not unset buffer from audio source: %s", device->getError());
		return;
	}

	// delete this source in OpenAL
	alDeleteSources(1, &id);

	if( device->checkError() )
	{
		LogWarn("Could not delete audio source: %s", device->getError());
		return;
	}
}

//-----------------------------------//

void AudioSource::play( const int count )
{
	// The source could be in three different states:
	//
	//		Stopped (in which case we play it from the beginning)
	//		Playing (in which case we do nothing)
	//		Paused (in which case we play from where it was paused)

	context->makeCurrent();

	if( isPlaying() )
	{
		// Source is already playing, do nothing.
		return;
	}
	else if( !isPaused() )
	{
		// Source is stopped so enqueue the buffer 'count' times
		//for(int i = 0; i < count; i++)
			//queue();

		alSourcei(id, AL_BUFFER, buffer->getId());
	}

	// Also handles the paused state implicitly.
	alSourcePlay(id);

	if(device->checkError())
	{
		LogWarn("Could not play audio source: %s", device->getError());
	}
}

//-----------------------------------//

void AudioSource::stop()
{
	context->makeCurrent();

	alSourceStop(id);

	if(device->checkError())
	{
		LogWarn("Could not stop audio source: %s", device->getError());
	}
}

//-----------------------------------//

void AudioSource::pause()
{
	context->makeCurrent();

	alSourcePause(id);

	if(device->checkError())
	{
		LogWarn( "Could not pause audio source: %s", device->getError());
	}
}

//-----------------------------------//

bool AudioSource::isPlaying()
{
	context->makeCurrent();

	ALenum state;

	alGetSourcei(id, AL_SOURCE_STATE, &state);

	return (state == AL_PLAYING);
}

//-----------------------------------//

bool AudioSource::isPaused()
{
	context->makeCurrent();

	ALenum state;

	alGetSourcei(id, AL_SOURCE_STATE, &state);

	return (state == AL_PAUSED);
}

//-----------------------------------//

void AudioSource::queue()
{
	context->makeCurrent();
	
	ALuint bufferId = buffer->getId();

	// Enqueue the buffer in the audio source.
	alSourceQueueBuffers(id, 1, &bufferId);

	if(device->checkError())
	{
		LogWarn( "Could not queue buffer in audio source: %s", device->getError());
	}
}

//-----------------------------------//

void AudioSource::clear()
{
	context->makeCurrent();
	
	//// dequeue all buffers in the audio source
	//alSourceUnqueueBuffers(id, 1, &bufferId);

	//if(device->checkError())
	//{
	//	LogWarn( "Could not queue buffer in audio source: %s",
	//		device->getError());
	//}
}

//-----------------------------------//

void AudioSource::setVolume(float volume)
{
	alSourcef(id, AL_GAIN, volume);
}

//-----------------------------------//

void AudioSource::setPitch(float pitch)
{
	alSourcef(id, AL_PITCH, pitch);
}

//-----------------------------------//

void AudioSource::setRolloff(float rolloff)
{
	alSourcef(id, AL_ROLLOFF_FACTOR, rolloff);
}

//-----------------------------------//

void AudioSource::setRolloffMode( RolloffMode::Enum mode )
{
	alDistanceModel(mode);
}

//-----------------------------------//

void AudioSource::setReferenceDistance( float distance )
{
	alSourcef(id, AL_REFERENCE_DISTANCE, distance);
}

//-----------------------------------//

void AudioSource::setMaxDistance( float distance )
{
	alSourcef(id, AL_MAX_DISTANCE, distance);
}

//-----------------------------------//

void AudioSource::setLoop(bool state)
{
	alSourcei(id, AL_LOOPING, state);
}

//-----------------------------------//

void AudioSource::setPosition( const Vector3& pos )
{
	context->makeCurrent();

	alSource3f(id, AL_POSITION, pos.x, pos.y, pos.z);

	if(device->checkError())
	{
		LogWarn( "Could not set position in audio source: %s", device->getError());
		return;
	}
}

//-----------------------------------//

NAMESPACE_ENGINE_END

#endif