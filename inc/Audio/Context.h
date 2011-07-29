/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#ifdef VAPOR_AUDIO_OPENAL

#include "Math/Vector.h"
#include "Audio/AL.h"

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

	friend class AudioSource;

public:

	AudioContext(AudioDevice* device);
	virtual ~AudioContext();

	// Set the global volume of this context.
	void setVolume( float volume );

	// Make this the current context in the audio device.
	void makeCurrent();

	// Sets the listener position.
	void setPosition(const Vector3& position);

	// Sets the listener orientation.
	void setOrientation(const Vector3& rotation);

protected:

	// Creates a new OpenAL context
	ALCcontext* createContext();

	// Checks if the last operation was successful
	bool checkError();	

	// Returns a string representation of an OpenAL context error
	const char* getError();

	// Audio device
	AudioDevice* device;

	// Holds an OpenAL context
	ALCcontext* context;

	// Holds the last OpenAL error code
	int error;
};

//-----------------------------------//

NAMESPACE_ENGINE_END

/** @} */

#endif