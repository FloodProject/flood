/************************************************************************
*
* vaporEngine (2008-2009)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#pragma once

#include "vapor/Platform.h"
#include "vapor/audio/Device.h"
#include "vapor/scene/Transform.h"

namespace vapor {
	namespace scene {

class Listener : public Transformable
{
public:

	Listener(audio::Device* device);
  
	// Set the global volume of this listener.
	void setVolume( float volume );

	// Make this the current listener in the audio device.
	void makeCurrent();

	virtual std::string save() { return ""; }

private:

	// Each listener is an OpenAL context
	ALCcontext* context;
};

} } // end namespaces