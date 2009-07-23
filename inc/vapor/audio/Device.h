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

#include <al.h>
#include <alc.h>

//#ifndef AL_ALC_H
//	struct ALCdevice;
//	struct ALCcontext;
//#endif

namespace vapor { 
	namespace audio {

/**
 * Audio device to play sound data using OpenAL as backend.
 */

class Device
{
	friend class Context;
	friend class Source;
	friend class Buffer;

public:

	Device();
	virtual ~Device();

	//// Play a possibly looped 2D sound
	//void play2D(shared_ptr<resources::Sound> sound, bool loop = false);

	// Sets the global audio volume
	void setVolume(float volume);

protected:

	// Switch current audio context.
	void switchContext(ALCcontext* context);

	// Gets the AL format matching the engine format
	int getALFormat(resources::SoundFormat::Enum format);
	
	// Prepares a buffer for AL usage
	shared_ptr<Buffer> prepareBuffer(shared_ptr<resources::Sound> sound);
	
	// Return the last error as a char array
	const char* getError();

	// Checks if the last operation was successful
	bool checkError();
	
	// Gets a string with the version of OpenAL
	std::string getVersion();
	
private:

	// Audio device
	ALCdevice* device;

	// Current audio context
	ALCcontext* ctx;

	// Holds the last error
	ALenum error;

	bool init;

	// Maps each sound to a OpenAL sound buffer id
	map<shared_ptr<resources::Sound>, shared_ptr<audio::Buffer> > soundBuffers;
};

} } // end namespaces

#endif