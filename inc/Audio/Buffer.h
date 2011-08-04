/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#ifdef ENABLE_AUDIO_OPENAL

#include "Resources/Sound.h"
#include "Audio/Source.h"
#include "Core/References.h"

NAMESPACE_ENGINE_BEGIN

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

class API_AUDIO AudioBuffer : public ReferenceCounted
{
	DECLARE_UNCOPYABLE(AudioBuffer)
	friend class AudioSource;

public:
	
	AudioBuffer( AudioDevice* device, Sound* sound );
	~AudioBuffer();
  
protected:

	// Gets the id of this buffer.
	GETTER(Id, ALuint, id)

	// Queues the buffer data in the source.
	void upload();

	// Holds a pointer to the audio device.
	AudioDevice* device;
	
	// Holds a pointer to the audio data buffer.
	Sound* sound;

	// Holds the source id from OpenAL.
	ALuint id;
};

TYPEDEF_INTRUSIVE_POINTER_FROM_TYPE( AudioBuffer );

//-----------------------------------//

NAMESPACE_ENGINE_END

#endif