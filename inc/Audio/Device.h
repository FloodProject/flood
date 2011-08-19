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
#include "Math/Vector.h"

FWD_DECL_INTRUSIVE(AudioBuffer)
FWD_DECL_INTRUSIVE(AudioContext)

typedef struct ALCdevice_struct ALCdevice;
typedef struct ALCcontext_struct ALCcontext;
typedef int ALenum;

NAMESPACE_ENGINE_BEGIN

//-----------------------------------//

/**
 * Audio device to play sound data using OpenAL as backend.
 */

typedef std::map<Sound*, AudioBufferPtr> SoundBufferMap;

class API_AUDIO AudioDevice
{
	DECLARE_UNCOPYABLE(AudioDevice)

	friend class AudioContext;
	friend class AudioSource;
	friend class AudioBuffer;

public:

	AudioDevice();
	~AudioDevice();

	// Gets a list of available devices.
	bool getDevices(std::vector<String>& devices);

	// Gets a list of available extensions.
	bool getExtensions(std::vector<String>& extensions);

	// Creates the internal AL device.
	bool createDevice(const String& device);

	// Creates the main context.
	bool createMainContext();
	
	// Sets the global audio volume
	void setVolume(float volume);

	// Gets the main audio context.
	GETTER(MainContext, AudioContext*, mainContext)

protected:

	// Gets the AL format matching the sound.
	int getFormat(Sound* sound);
	
	// Prepares a buffer for AL usage.
	AudioBufferPtr prepareBuffer(Sound* sound);
	
	// Return the last error as a char array.
	const char* getError();

	// Checks if the last operation was successful.
	bool checkError();
	
	// Gets a string with the version of OpenAL.
	const String getVersion();

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

// Gets the main audio device.
API_AUDIO AudioDevice* GetAudioDevice();

//-----------------------------------//

NAMESPACE_ENGINE_END

#endif