/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#ifdef ENABLE_AUDIO_OPENAL

#include "Math/Vector.h"

typedef struct ALCdevice_struct ALCdevice;
typedef struct ALCcontext_struct ALCcontext;

FWD_DECL_INTRUSIVE(AudioSource)

NAMESPACE_ENGINE_BEGIN

//-----------------------------------//

class AudioDevice;

/**
 * Wraps an OpenAL context in a class. A context in OpenAL is like a context 
 * in OpenGL, in that it's properties are independent of of other contexts. 
 * Each listener in the scene graph will match a context in the audio device, 
 * so that we can play sounds from  different positions without changing 
 * everything in a context.
 */

class API_AUDIO AudioContext
{
	DECLARE_UNCOPYABLE(AudioContext)

public:

	AudioContext(ALCcontext* context);
	~AudioContext();

	// Creates a new audio source.
	AudioSourcePtr createSource();

	// Set the global volume of this context.
	void setVolume( float volume );

	// Make this the current context in the audio device.
	void makeCurrent();

	// Processes the context.
	void process();

	// Sets the listener position.
	void setPosition(const Vector3& position);

	// Sets the listener orientation.
	void setOrientation(const Vector3& rotation);

protected:

	// Holds an OpenAL context
	ALCcontext* context;

	// Holds the last OpenAL error code
	int error;
};

//-----------------------------------//

NAMESPACE_ENGINE_END

#endif