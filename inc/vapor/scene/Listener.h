/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#ifdef VAPOR_AUDIO_OPENAL

#include "scene/Component.h"
#include "audio/Context.h"

namespace vapor {

//-----------------------------------//

class VAPOR_API Listener : Component
{
	DECLARE_UNCOPYABLE(Listener)
	DECLARE_CLASS_()

public:

	Listener();
	virtual ~Listener();

	// Set the volume of this listener.
	void setVolume( float volume );

	// Make this the current context in the audio device.
	void makeCurrent();

	// Updates the component.
	virtual void update( double delta );

protected:

	// Audio context.
	AudioContext* audioContext;

	// Volume level.
	float volume;
};

TYPEDEF_SHARED_POINTER_FROM_TYPE( Listener );

//-----------------------------------//

} // end namespace

#endif