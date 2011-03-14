/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#ifdef VAPOR_AUDIO_OPENAL

#include "Resources/Sound.h"
#include "Audio/Source.h"

namespace vapor {

//-----------------------------------//

class AudioDevice;
class AudioContext;

/**
 * Wraps an OpenAL buffer in a class. A buffer in OpenAL is the object 
 * that contains the audio data. This will hold the id to the data and
 * delete it when no other source needs it.
 *
 * TODO: Add streaming audio and have some caching strategy so it does
 * not delete the audio data if it's potentially needed in the future.
 */

class VAPOR_API AudioBuffer
{
	DECLARE_UNCOPYABLE(AudioBuffer)

	friend class AudioSource;

public:
	
	AudioBuffer( AudioDevice* device, const SoundPtr& sound );
	~AudioBuffer();
  
protected:

	// Gets the id of this buffer.
	GETTER(Id, ALuint, id)

	// Queues the buffer data in the source.
	void upload();

	// Holds a pointer to the audio device.
	AudioDevice* device;
	
	// Holds a pointer to the audio data buffer.
	SoundPtr resource;

	// Holds the source id from OpenAL.
	ALuint id;
};

TYPEDEF_SHARED_POINTER_FROM_TYPE( AudioBuffer );

//-----------------------------------//

} // end namespace

#endif