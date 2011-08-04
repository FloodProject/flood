/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Engine/API.h"

#ifdef ENABLE_AUDIO_OPENAL

#include "Audio/Buffer.h"
#include "Audio/Device.h"

NAMESPACE_ENGINE_BEGIN

//-----------------------------------//

AudioBuffer::AudioBuffer( AudioDevice* device, Sound* sound )
	: device(device)
	, sound(sound)
	, id(0)
{
	alGenBuffers(1, &id);

	// Check if the buffer was successfuly created.
	if(device->checkError()) 
	{
		LogWarn( "Error creating a sound buffer: %s", device->getError());
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
	const std::vector<byte>& buffer = sound->getBuffer();
	ALsizei size = (ALsizei) buffer.size();
	
	int frequency = sound->getFrequency();
	int format = device->getFormat(sound);

	if( buffer.empty() ) return;

	// Uploads sound data to the buffer.
	alBufferData( id, format, &buffer[0], size, frequency );
	
	if(device->checkError())
	{
		LogWarn("Error uploading sound to buffer: %s", device->getError());
		return;
	}
}

//-----------------------------------//

NAMESPACE_ENGINE_END

#endif