/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#ifdef VAPOR_AUDIO_OPENAL

#include "resources/Sound.h"
#include "math/Vector3.h"

#include <al.h>
#include <alc.h>

FWD_DECL_SHARED(AudioBuffer)

namespace vapor {

//-----------------------------------//

/**
 * Audio device to play sound data using OpenAL as backend.
 */

typedef std::map<SoundPtr, AudioBufferPtr> SoundBufferMap;

class VAPOR_API AudioDevice
{
	DECLARE_UNCOPYABLE(AudioDevice)

	friend class AudioContext;
	friend class AudioSource;
	friend class AudioBuffer;

public:

	AudioDevice();
	virtual ~AudioDevice();
	
	// Sets the global audio volume
	void setVolume(float volume);

	// Gets the main audio context.
	GETTER(MainContext, AudioContext*, mainContext)

protected:

	// Switch current audio context.
	void switchContext(ALCcontext* context);

	// Gets the AL format matching the sound.
	int getFormat(const SoundPtr& sound);
	
	// Prepares a buffer for AL usage.
	AudioBufferPtr prepareBuffer(const SoundPtr& sound);
	
	// Return the last error as a char array.
	const char* getError();

	// Checks if the last operation was successful.
	bool checkError();
	
	// Gets a string with the version of OpenAL.
	const std::string getVersion();

	// Audio device.
	ALCdevice* device;

	// Current audio context.
	ALCcontext* context;

	// Holds the last error
	ALenum error;

	// Initialization state.
	bool init;

	// Maps each sound to a OpenAL sound buffer.
	SoundBufferMap soundBuffers;

	// Main audio context.
	AudioContext* mainContext;
};

//-----------------------------------//

} // end namespace

#endif