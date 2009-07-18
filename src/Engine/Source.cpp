/************************************************************************
*
* vaporEngine (2008-2009)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#include "vapor/audio/Source.h"

namespace vapor {
	namespace audio {

//-----------------------------------//

Source::Source(audio::Context* context, shared_ptr<resources::Sound> sound)
	: context(context), resource(sound), sourceId(0)
{	
	context->makeCurrent();

	device = context->device;

	// generate a new OpenAL source
	alGenSources(1, &sourceId);

	if(device->checkError())
	{
		warn("audio::al", "Could not generate a new audio source: %s",
			device->getError());
	}

	math::Vector3 zero(0.0f, 0.0f, 0.0f);
	setPosition(zero);
}

//-----------------------------------//

Source::~Source()
{
	context->makeCurrent();

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

	// enqueue the buffer 'count' times
	for(int i = 0; i < count; i++)
		queue();

	alSourcePlay(sourceId);

	if(device->checkError())
	{
		warn("audio::al", "Could not play audio source: %s",
			device->getError());
	}
}

//-----------------------------------//

void Source::queue()
{
	context->makeCurrent();
	
	// get the OpenAL sound buffer id from the device
	ALuint bufferId = device->prepareBuffer(resource);

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