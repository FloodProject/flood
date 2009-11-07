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

/** \addtogroup audio Audio 
 * @{ */

namespace vapor {
	namespace audio {

//-----------------------------------//

/**
 * Audio device to play sound data using OpenAL as backend.
 */

class VAPOR_API Device : private boost::noncopyable
{
	friend class Context;
	friend class Source;
	friend class Buffer;

public:

	Device();
	virtual ~Device();
	
	// Play a possibly looped 2D sound
	//void play2D(shared_ptr<resources::Sound> sound, bool loop = false);

	// Sets the global audio volume
	void setVolume(float volume);

protected:

	// Switch current audio context.
	void switchContext(ALCcontext* context);

	// Gets the AL format matching the engine format.
	int getALFormat(resources::SoundFormat::Enum format);
	
	// Prepares a buffer for AL usage.
	std::shared_ptr<Buffer> prepareBuffer( std::shared_ptr<resources::Sound> sound );
	
	// Return the last error as a char array.
	const char* getError();

	// Checks if the last operation was successful.
	bool checkError();
	
	// Gets a string with the version of OpenAL.
	const std::string getVersion();
	
private:

	// Audio device
	ALCdevice* device;

	// Current audio context
	ALCcontext* ctx;

	// Holds the last error
	ALenum error;

	bool init;

	// Maps each sound to a OpenAL sound buffer id
	std::map< std::shared_ptr<resources::Sound>, std::shared_ptr<audio::Buffer> > soundBuffers;
};

//-----------------------------------//

#else

// declare audio namespace so using declarations don't
// get broken in user code.

namespace vapor { 
	namespace audio {

#endif

//-----------------------------------//

} } // end namespaces

/** @} */