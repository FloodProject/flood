/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "vapor/PCH.h"

#ifdef VAPOR_AUDIO_OPENAL

#include "audio/Buffer.h"
#include "audio/Device.h"

namespace vapor {

//-----------------------------------//

AudioBuffer::AudioBuffer( AudioDevice* device, const SoundPtr& sound )
	: device(device)
	, id(0)
	, resource(sound)
{
	alGenBuffers(1, &id);

	// Check if the buffer was successfuly created.
	if(device->checkError()) 
	{
		Log::warn( "Error creating a sound buffer: %s", device->getError());
	}

	upload();
}

//-----------------------------------//

AudioBuffer::~AudioBuffer()
{
	alDeleteBuffers(1, &id);

	//if(device->soundBuffers.find(resource) != device->soundBuffers.end())
	//{
	//	device->soundBuffers.erase(resource);
	//}
}

//-----------------------------------//

void AudioBuffer::upload()
{
	const std::vector<byte>& buffer = resource->getBuffer();
	ALsizei size = (ALsizei) buffer.size();
	
	int frequency = resource->getFrequency();
	int format = device->getFormat(resource);

	if( buffer.empty() )
		return;

	// Uploads sound data to the buffer.
	alBufferData( id, format, &buffer[0], size, frequency );
	
	if(device->checkError())
		Log::warn("Error uploading sound to buffer: %s", device->getError());
}

//-----------------------------------//

} // end namespace

#endif