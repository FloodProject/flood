/************************************************************************
*
* vaporEngine (2008-2009)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#include "vapor/PCH.h"

#ifdef VAPOR_AUDIO_OPENAL

#include "vapor/audio/Source.h"

namespace vapor {
	namespace audio {

//-----------------------------------//

Source::Source(tr1::shared_ptr<audio::Context> context, resources::SoundPtr sound)
	: context(context), device(context->device), sourceId(0)
{	
	context->makeCurrent();

	// generate a new OpenAL source
	alGenSources(1, &sourceId);

	if(device->checkError())
	{
		warn("audio::al", "Could not generate a new audio source: %s",
			device->getError());
	}

	buffer = device->prepareBuffer(sound);
	setPosition( math::Vector3::Zero );
}

//-----------------------------------//

Source::~Source()
{
	context->makeCurrent();

	stop();

	alSourcei(sourceId, AL_BUFFER, AL_NONE);

	if(device->checkError())
	{
		warn("audio::al", "Could not unset buffer from audio source: %s",
			device->getError());
	}

	// delete this source in OpenAL
	alDeleteSources(1, &sourceId);

	if(device->checkError())
	{
		warn("audio::al", "Could not delete audio source: %s",
			device->getError());
	}
}

//-----------------------------------//

void Source::play( const int count )
{
	/**
	 * The source could be in three different states:
	 *
	 *		Stopped (in which case we play it from the beginning)
	 *		Playing (in which case we do nothing)
	 *		Paused (in which case we play from where it was paused)
	 * 
	 * We'll handle each case now.
	 */

	context->makeCurrent();

	if( isPlaying() )
	{
		// Source is already playing, do nothing.
		return;
	}

	if( !isPaused() )
	{
		// Source is stopped so enqueue the buffer 'count' times
		//for(int i = 0; i < count; i++)
			//queue();

		alSourcei(sourceId, AL_BUFFER, buffer->id());
	}

	// Also handles the Paused state implicitly.
	alSourcePlay(sourceId);

	if(device->checkError())
	{
		warn("audio::al", "Could not play audio source: %s",
			device->getError());
	}
}

//-----------------------------------//

void Source::stop()
{
	context->makeCurrent();

	alSourceStop(sourceId);

	if(device->checkError())
	{
		warn("audio::al", "Could not stop audio source: %s",
			device->getError());
	}
}

//-----------------------------------//

void Source::pause()
{
	context->makeCurrent();

	alSourcePause(sourceId);

	if(device->checkError())
	{
		warn("audio::al", "Could not pause audio source: %s",
			device->getError());
	}
}

//-----------------------------------//

bool Source::isPlaying()
{
	context->makeCurrent();

	ALenum state;

	alGetSourcei(sourceId, AL_SOURCE_STATE, &state);

	return (state == AL_PLAYING);
}

//-----------------------------------//

bool Source::isPaused()
{
	context->makeCurrent();

	ALenum state;

	alGetSourcei(sourceId, AL_SOURCE_STATE, &state);

	return (state == AL_PAUSED);
}

//-----------------------------------//

void Source::queue()
{
	context->makeCurrent();
	
	// get the OpenAL sound buffer id from the device
	ALuint bufferId = buffer->id();

	// enqueue the buffer in the audio source
	alSourceQueueBuffers(sourceId, 1, &bufferId);

	if(device->checkError())
	{
		warn("audio::al", "Could not queue buffer in audio source: %s",
			device->getError());
	}
}

//-----------------------------------//

void Source::clear()
{
	context->makeCurrent();
	
	// get the OpenAL sound buffer id from the device
	ALuint bufferId = buffer->id();

	//// dequeue all buffers in the audio source
	//alSourceUnqueueBuffers(sourceId, 1, &bufferId);

	//if(device->checkError())
	//{
	//	warn("audio::al", "Could not queue buffer in audio source: %s",
	//		device->getError());
	//}
}

//-----------------------------------//

void Source::setPosition( const math::Vector3& pos )
{
	context->makeCurrent();

	alSource3f(sourceId, AL_POSITION, pos.x, pos.y, pos.z);

	if(device->checkError())
	{
		warn("audio::al", "Could not set position in audio source: %s",
			device->getError());
	}
}

//-----------------------------------//

} } // end namespaces

#endif