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

#include "vapor/scene/Listener.h"
#include "vapor/scene/Sound.h"

#include <al.h>
#include <alc.h>

namespace vapor {
	namespace audio {

/**
 * Audio device to play sound data using OpenAL as backend.
 */

class AL
{
public:

	AL();
	virtual ~AL();

	// Play a possibly looped 2D sound
	void play2D(const resources::Sound* sound, bool loop = false);

	// Sets the global audio volume
	void setVolume(float volume);

	// Switch the current listener
	void switchListener(scene::Listener* listener);

protected:

	// Sets the listener position
	void setListener(const math::Vector3 position);

	ALint getALFormat(resources::SoundFormat::Enum format);
	ALuint prepareBuffer(const resources::Sound* sound);
	
private:

	// Audio device
	ALCdevice* device;

	// Maps each sound to a OpenAL sound buffer id
	map<const resources::Sound*, ALuint> soundBuffers;

	// Holds the sources for a given file
	//map<const scene::Sound*, ALuint> soundSources;

	// Maps each listener node to a unique OpenAL context
	map<scene::Listener*, ALCcontext*> listenerContexts;
};

} } // end namespaces

#endif