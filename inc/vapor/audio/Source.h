/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#ifdef VAPOR_AUDIO_OPENAL

#include "Audio/Device.h"
#include "Audio/Context.h"
#include "Resources/Sound.h"
#include "Math/Vector.h"

FWD_DECL_SHARED(AudioBuffer)

namespace vapor {

//-----------------------------------//

struct VAPOR_API RolloffMode
{
	DECLARE_ENUM()

	enum Enum
	{
		Logarithmic = AL_INVERSE_DISTANCE_CLAMPED,
		Linear = AL_LINEAR_DISTANCE_CLAMPED,
		Exponential = AL_EXPONENT_DISTANCE_CLAMPED
	};
};

//-----------------------------------//

class AudioContext;

/**
 * Wraps an OpenAL source in a class. A source in OpenAL is the object 
 * that contains the position of the sound being played in 2D, and also 
 * tells what audio data to play. Each source will get a shared audio
 * buffer from the audio device.
 */

class VAPOR_API AudioSource
{
	DECLARE_UNCOPYABLE(AudioSource)

public:

	AudioSource( AudioContext* context, const SoundPtr& sound );
	~AudioSource();

	// Plays the sound buffer resuming if paused.
	void play( int timesToPlay = 1 );

	// Stops the sound buffer and seeks to the beginning.
	void stop();

	// Pauses the sound buffer.
	void pause();

	// Checks if the source is currently playing.
	bool isPlaying();

	// Checks if the source is currently paused.
	bool isPaused();

	// Sets the volume of the source. Volume is in the range [0.0-1.0].
	void setVolume( float volume );

	// Sets the pitch of the source. Pitch is in the range [0.0-1.0].
	void setPitch( float pitch );

	// Sets the reference distance.
	void setReferenceDistance( float distance );

	// Sets the maximum distance.
	void setMaxDistance( float distance );

	// Sets the rolloff mode of the source.
	void setRolloffMode( RolloffMode::Enum rolloff );

	// Sets the roll-off of the source. Roll-off is in the range [0.0-1.0].
	void setRolloff( float rolloff );

	// Sets the loop of the source.
	void setLoop( bool state );

	// Sets the position of the source.
	void setPosition( const Vector3& position );
  
protected:

	// Queues the buffer data in the source.
	void queue();

	// Clears the buffer queue.
	void clear();

	// Holds a pointer to the audio device.
	AudioDevice* device;

	// Holds a pointer to the audio context.
	AudioContext* context;
	
	// Holds a pointer to the audio data buffer.
	AudioBufferPtr buffer;

	// Holds the source id from OpenAL.
	ALuint id;
};

TYPEDEF_SHARED_POINTER_FROM_TYPE( AudioSource );

//-----------------------------------//

} // end namespace

#endif