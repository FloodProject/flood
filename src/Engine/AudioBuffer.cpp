/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "vapor/PCH.h"

#ifdef VAPOR_AUDIO_OPENAL

#include "vapor/audio/Buffer.h"
#include "vapor/audio/Device.h"

namespace vapor { namespace audio {

//-----------------------------------//

Buffer::Buffer( audio::Device* device, resources::SoundPtr sound )
	: device(device), bufferId(0), resource(sound)
{
	alGenBuffers(1, &bufferId);

	// check if buffer was successfuly created
	if(device->checkError()) 
	{
		warn("audio::al", "Error creating a sound buffer: %s",
			device->getError());
	}

	upload();
}

//-----------------------------------//

Buffer::~Buffer()
{
	alDeleteBuffers(1, &bufferId);

	//if(device->soundBuffers.find(resource) 
	//	!= device->soundBuffers.end())
	//{
	//	device->soundBuffers.erase(resource);
	//}
}

//-----------------------------------//

void Buffer::upload()
{
	// upload sound data to buffer
	alBufferData(bufferId, 
		device->getALFormat( resource->getFormat()), &resource->getBuffer()[0], 
		static_cast <ALsizei> ( resource->getBuffer().size() ), resource->getFrequency() );
	
	if(device->checkError())
	{
		warn("audio::al", "Error uploading sound to buffer: %s",
			device->getError());
	}	
}

//-----------------------------------//

ALuint Buffer::id()
{
	return bufferId;
}

//-----------------------------------//

} } // end namespaces

#endif