/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#ifdef VAPOR_AUDIO_OPENAL

#include "vapor/resources/Sound.h"
#include "vapor/audio/Device.h"
#include "vapor/audio/Source.h"

/** \addtogroup audio Audio 
 * @{ */

namespace vapor { namespace audio {

//-----------------------------------//

/**
 * Wraps an OpenAL buffer in a class. A buffer in OpenAL is the object 
 * that contains the audio data. This will hold the id to the data and
 * delete it when no other source needs it.
 *
 * TODO: Streaming audio and maybe have some caching strategy so it
 * doesn't delete the audio data if it's potentially needed in the future.
 */

class VAPOR_API Buffer : private boost::noncopyable
{
	friend class Source;

public:
	
	/// Constructor
	Buffer( audio::Device* device, resources::SoundPtr sound );
	
	/// Destructor
	~Buffer();
  
protected:

	/// Gets the OpenAL id of this buffer.
	ALuint id();

	/// Queues the buffer data in the source.
	void upload();

	/// Holds a pointer to the audio device.
	audio::Device* device;
	
	/// Holds a pointer to the audio data buffer.
	resources::SoundPtr resource;

	/// Holds the source id from OpenAL.
	ALuint bufferId;
};

//-----------------------------------//

TYPEDEF_SHARED_POINTER_FROM_TYPE( Buffer );

//-----------------------------------//

} } // end namespaces

/** @} */

#endif