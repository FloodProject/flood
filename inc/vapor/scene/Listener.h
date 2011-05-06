/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#ifdef VAPOR_AUDIO_OPENAL

#include "Scene/Component.h"
#include "Audio/Context.h"

namespace vapor {

//-----------------------------------//

REFLECT_DECLARE_CLASS(Listener)

class VAPOR_API Listener : public Component
{
	DECLARE_UNCOPYABLE(Listener)
	REFLECT_DECLARE_OBJECT(Listener)

public:

	Listener();
	virtual ~Listener();

	// Set the volume of this listener.
	void setVolume( float volume );

	// Make this the current context in the audio device.
	void makeCurrent();

	// Updates the component.
	virtual void update( float delta );

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