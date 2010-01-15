/************************************************************************
*
* vaporEngine (2008-2010)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#pragma once

#include "vapor/CompileOptions.h"

#ifdef VAPOR_AUDIO_OPENAL

#include "vapor/Platform.h"

#include "vapor/audio/Device.h"
#include "vapor/audio/Context.h"
#include "vapor/audio/Buffer.h"
#include "vapor/resources/Sound.h"
#include "vapor/math/Vector3.h"

namespace vapor { namespace audio {

/** \addtogroup audio Audio 
 * @{ */

//-----------------------------------//

/**
 * Wraps an OpenAL source in a class. A source in OpenAL is the object 
 * that contains the position of the sound being played in 2D, and also 
 * tells what audio data to play. Each source will get a shared audio
 * buffer from the audio device.
 */

class VAPOR_API Source : private boost::noncopyable
{
public:

	Source( std::shared_ptr<audio::Context> context, resources::SoundPtr sound );
	~Source();

	//void setResource( shared_ptr<resources::Resource> sound );

	// Plays the sound buffer a number of times. If you paused the source
	// then it will resume from where it was paused.
	void play( const int count = 1 );

	// Stops the playing of the audio. The next time you play it will start
	// from the beginning.
	void stop();

	// Pauses the playing of the audio. The next time you play it will start
	// from the position it had when it was paused.
	void pause();

	// Checks if the source is currently playing.
	bool isPlaying();

	// Checks if the source is currently paused.
	bool isPaused();

	// Sets the volume of the source. Volume is in the range [0.0-1.0].
	void setVolume( const float volume );

	// Sets the pitch of the sourc. Pitch is in the range [0.0-1.0].
	void setPitch( const float pitch );

	// Sets the roll-off of the sourc. Roll-off is in the range [0.0-1.0].
	void setRollOff( const float rollOff );

	// Sets the position of the source.
	void setPosition( const math::Vector3& position ); 
  
protected:

	// Queues the buffer data in the source.
	void queue();

	// Clears the buffer queue.
	void clear();

	// Holds a pointer to the audio device.
	audio::Device* device;

	// Holds a pointer to the audio context.
	std::shared_ptr<audio::Context> context;
	
	// Holds a pointer to the audio data buffer.
	std::shared_ptr<Buffer> buffer;

	// Holds the source id from OpenAL.
	ALuint sourceId;
};

//-----------------------------------//

} } // end namespaces

/** @} */

#endif