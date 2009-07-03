/************************************************************************
*
* vaporEngine (2008-2009)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#pragma once

#include "vapor/CompileOptions.h"

#ifdef VAPOR_AUDIO_OPENAL

#include "vapor/Platform.h"

#include "vapor/math/Vector3.h"
#include "vapor/resources/Sound.h"

#ifndef AL_ALC_H
	struct ALCdevice;
	struct ALCcontext;
#endif

namespace vapor {

	namespace scene {
		class Listener;
		class Sound;
	} // end namespace

	namespace audio {

/**
 * Audio device to play sound data using OpenAL as backend.
 */

class Device
{
	friend class vapor::scene::Listener;
	friend class vapor::scene::Sound;

public:

	Device();
	virtual ~Device();

	// Play a possibly looped 2D sound
	void play2D(const resources::Sound* sound, bool loop = false);

	// Sets the global audio volume
	void setVolume(float volume);

	// Switch the current listener
	void switchListener(scene::Listener* listener);

protected:

	// Sets the listener position
	void setListener(const math::Vector3& position);

	// Gets the AL format matching the engine format
	int getALFormat(resources::SoundFormat::Enum format);
	
	// Prepares a buffer for AL usage
	uint prepareBuffer(const resources::Sound* sound);
	
	// Return the last error as a char array
	const char* getError();

	// Checks if the last operation was successful
	bool checkError();
	
private:

	// Audio device
	ALCdevice* device;

	// Current audio context
	ALCcontext* ctx;

	// Holds the last error
	int error;

	bool init;

	// Maps each sound to a OpenAL sound buffer id
	map<const resources::Sound*, uint> soundBuffers;

	// Holds the sources for a given file
	//map<const scene::Sound*, ALuint> soundSources;

	// Maps each listener node to a unique OpenAL context
	map<scene::Listener*, ALCcontext*> listenerContexts;
};

} } // end namespaces

#endif