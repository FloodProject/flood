/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

#ifdef ENABLE_AUDIO_OPENAL

#include "Engine/Scene/Component.h"
#include "Engine/Audio/Context.h"

NAMESPACE_ENGINE_BEGIN

//-----------------------------------//

API_SCENE REFLECT_DECLARE_CLASS(Listener)

class API_SCENE Listener : public Component
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

TYPEDEF_INTRUSIVE_POINTER_FROM_TYPE( Listener );

//-----------------------------------//

NAMESPACE_ENGINE_END

#endif