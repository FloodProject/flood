/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#ifdef ENABLE_AUDIO_OPENAL

#include "Resources/Sound.h"
#include "Math/Vector.h"

FWD_DECL_INTRUSIVE(AudioBuffer)
typedef unsigned int ALuint;

NAMESPACE_ENGINE_BEGIN

//-----------------------------------//

REFLECT_DECLARE_ENUM(RolloffMode)

#define AL_INVERSE_DISTANCE_CLAMPED               0xD002
#define AL_LINEAR_DISTANCE_CLAMPED                0xD004
#define AL_EXPONENT_DISTANCE_CLAMPED              0xD006

struct API_AUDIO RolloffMode
{
	enum Enum
	{
		Logarithmic = AL_INVERSE_DISTANCE_CLAMPED,
		Linear = AL_LINEAR_DISTANCE_CLAMPED,
		Exponential = AL_EXPONENT_DISTANCE_CLAMPED
	};
};

//-----------------------------------//

class AudioContext;
class AudioDevice;

/**
 * Wraps an OpenAL source in a class. A source in OpenAL is the object 
 * that contains the position of the sound being played in 2D, and also 
 * tells what audio data to play. Each source will get a shared audio
 * buffer from the audio device.
 */

class API_AUDIO AudioSource
{
	DECLARE_UNCOPYABLE(AudioSource)

public:

	AudioSource( AudioContext* context, const SoundHandle& sound );
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

	// Callback when buffer is uploaded.
	void onBufferUploaded(AudioBuffer*);

	// Holds if the source is pending play.
	bool isPendingPlay;

	// Holds a pointer to the audio device.
	AudioDevice* device;

	// Holds a pointer to the audio context.
	AudioContext* context;
	
	// Holds a pointer to the audio data buffer.
	AudioBufferPtr buffer;

	// Holds the source id from OpenAL.
	ALuint id;
};

TYPEDEF_INTRUSIVE_POINTER_FROM_TYPE( AudioSource );

//-----------------------------------//

NAMESPACE_ENGINE_END

#endif