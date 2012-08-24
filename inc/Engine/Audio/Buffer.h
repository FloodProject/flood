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

// Buffers data into an audio buffer.
struct AudioBufferDetails;
API_AUDIO void AudioBufferData(ALuint buffer, const AudioBufferDetails& details);

// Uploads the sound into the buffer.
API_AUDIO void AudioBufferSound(AudioBuffer* buffer, Sound* sound);

//-----------------------------------//

struct AudioBufferDetails
{
	uint8* data;
	size_t size;
	int frequency;
	int format;
};

// Gets the buffer data details for a sound.
API_AUDIO void AudioGetBufferDataDetails(AudioBufferDetails& details, Sound* sound);

//-----------------------------------//

/**
 * Wraps an OpenAL buffer, the object that contains the audio data.
 * This will hold the id to the data and delete it when no other
 * source needs it.
 */

class API_AUDIO AudioBuffer : public ReferenceCounted
{
	DECLARE_UNCOPYABLE(AudioBuffer)
	friend class AudioSource;

public:

	AudioBuffer( AudioDevice* device );
	~AudioBuffer();

	// Gets the id of this buffer.
	GETTER(Id, ALuint, id)

	// Gets if the buffer is uploaded.
	GETTER(Uploaded, bool, uploaded)

	// Uploads the the buffer data in the source.
	void upload(const AudioBufferDetails& details);

	// Holds a pointer to the audio device.
	AudioDevice* device;
	
	// Holds if the buffer has been uploaded.
	bool uploaded;

	// Holds the buffer id from OpenAL.
	ALuint id;

	// Event is sent when buffer is uploaded.
	Event1<AudioBuffer*> onBufferUploaded;
};

TYPEDEF_INTRUSIVE_POINTER_FROM_TYPE( AudioBuffer );

//-----------------------------------//

NAMESPACE_ENGINE_END

#endif