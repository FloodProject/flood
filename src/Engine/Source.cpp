/************************************************************************
*
* vaporEngine (2008-2009)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#include "vapor/CompileOptions.h"

#ifdef VAPOR_AUDIO_OPENAL

#include "vapor/audio/Source.h"

namespace vapor {
	namespace audio {

//-----------------------------------//

Source::Source(shared_ptr<audio::Context> context, shared_ptr<resources::Sound> sound)
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

	math::Vector3 zero(0.0f, 0.0f, 0.0f);
	setPosition(zero);
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

void Source::play(int count)
{
	context->makeCurrent();

	if( isPlaying() ) return;

	if( !isPaused() )
	{
		// enqueue the buffer 'count' times
		for(int i = 0; i < count; i++)
			queue();
	}

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

	if (state == AL_PLAYING)
	  return true;

	return false;
}

//-----------------------------------//

bool Source::isPaused()
{
	context->makeCurrent();

	ALenum state;

	alGetSourcei(sourceId, AL_SOURCE_STATE, &state);

	if (state == AL_PAUSED)
	  return true;

	return false;
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

void Source::setPosition( math::Vector3& pos )
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