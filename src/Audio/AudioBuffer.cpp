/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "vapor/PCH.h"

#ifdef VAPOR_AUDIO_OPENAL

#include "Audio/Buffer.h"
#include "Audio/Device.h"

namespace vapor {

//-----------------------------------//

AudioBuffer::AudioBuffer( AudioDevice* device, const SoundPtr& sound )
	: device(device)
	, resource(sound)
	, id(0)
{
	alGenBuffers(1, &id);

	// Check if the buffer was successfuly created.
	if(device->checkError()) 
	{
		Log::warn( "Error creating a sound buffer: %s", device->getError());
		return;
	}

	upload();
}

//-----------------------------------//

AudioBuffer::~AudioBuffer()
{
	alDeleteBuffers(1, &id);

#if 0
	if(device->soundBuffers.find(resource) != device->soundBuffers.end())
	{
		device->soundBuffers.erase(resource);
	}
#endif
}

//-----------------------------------//

void AudioBuffer::upload()
{
	const std::vector<byte>& buffer = resource->getBuffer();
	ALsizei size = (ALsizei) buffer.size();
	
	int frequency = resource->getFrequency();
	int format = device->getFormat(resource);

	if( buffer.empty() ) return;

	// Uploads sound data to the buffer.
	alBufferData( id, format, &buffer[0], size, frequency );
	
	if(device->checkError())
	{
		Log::warn("Error uploading sound to buffer: %s", device->getError());
		return;
	}
}

//-----------------------------------//

} // end namespace

#endif