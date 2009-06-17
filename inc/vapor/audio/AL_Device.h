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

namespace vapor {
	namespace audio {

/**
 * Plays sound data using OpenAL as backend.
 */

class AL
{
public:

	AL();
	virtual ~AL();

	// Sets the listener position
	void setListener(const math::Vector3 position);

	// Play a possibly looped 2D sound
	void play2D(const resources::Sound* sound, bool loop = false);

	// Sets the global audio volume
	void setVolume(float volume);

protected:

	ALint getALFormat(resources::SoundFormat::Enum format);
	ALuint prepareBuffer(const resources::Sound* sound);
	
private:

	// OpenAL stuff
	ALCdevice* device;
	ALCcontext* context;

	// Maps each sound to a OpenAL sound buffer id
	map<const resources::Sound*, ALuint> soundBuffers;

	// Holds the sources for a given file
	map<const resources::Sound*, vector<ALuint>> soundSources;

	math::Vector3 listenerPosition;
};

} } // end namespaces

#endif