/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Engine/API.h"

#ifdef ENABLE_AUDIO_OPENAL

#include "Audio/Source.h"
#include "Audio/Buffer.h"
#include "Audio/Context.h"
#include "Audio/Device.h"
#include "Audio/AL.h"

#define LogAudio(s) (LogWarn(s ": %s", AudioGetError()))

NAMESPACE_ENGINE_BEGIN

//-----------------------------------//

REFLECT_ENUM(RolloffMode)
	ENUM(Logarithmic)
	ENUM(Linear)
	ENUM(Exponential)
REFLECT_ENUM_END()

//-----------------------------------//

AudioSource::AudioSource(AudioContext* context)
	: context(context)
	, id(0)
{
	for( size_t i = 0; i < AudioSourceNumBuffers; ++i )
		buffers[i] = nullptr;

	// Generates a new OpenAL source.
	alGenSources(1, &id);

	if(AudioCheckError())
	{
		LogAudio("Could not generate a new audio source");
		return;
	}

	setPosition( Vector3::Zero );
}

//-----------------------------------//

AudioSource::~AudioSource()
{
	context->makeCurrent();

	stop();
	empty();

	alSourcei(id, AL_BUFFER, AL_NONE);
	
	if( AudioCheckError() )
	{
		LogAudio("Could not unset buffer from audio source");
		return;
	}

	// Delete the source from OpenAL.
	alDeleteSources(1, &id);

	if( AudioCheckError() )
	{
		LogAudio("Could not delete audio source");
		return;
	}
}

//-----------------------------------//

void AudioSource::empty()
{
	int queued;
	alGetSourcei(id, AL_BUFFERS_QUEUED, &queued);

	while(queued--)
	{
		ALuint buffer;

		alSourceUnqueueBuffers(id, 1, &buffer);
		
		if(AudioCheckError())
			LogAudio("Could not unqueue buffer from audio source");
	}
}

//-----------------------------------//

void AudioSource::queue()
{
	for(size_t i = 0; i < AudioSourceNumBuffers; ++i)
	{
		AudioBuffer* buffer = buffers[i].get();
		if( !buffer ) break;

		ALuint bufferId = buffer->getId();

		// Enqueue the buffer in the audio source.
		alSourceQueueBuffers(id, 1, &bufferId);

		if(AudioCheckError())
			LogAudio( "Could not queue buffer in audio source");
	}
}

//-----------------------------------//

static const size_t BUFFER_SIZE = 8192 * 4;

static bool AudioStreamQueueBuffer(ALuint source, ALuint buffer,
	AudioBufferDetails& details, ResourceStream& stream, bool loop)
{
	uint8 data[BUFFER_SIZE];

	bool hasMoreData = true;

	int size = stream.decode(data, ARRAY_SIZE(data));
	
	if( size < BUFFER_SIZE )
		hasMoreData = false;

	details.data = data;
	details.size = size;

	AudioBufferData(buffer, details);
	
	alSourceQueueBuffers(source, 1, &buffer);

	if(AudioCheckError())
		LogAudio("Could not queue buffer in audio source");

	// Reset the stream so it loops properly.
	if( !hasMoreData && loop )
	{
		stream.reset();
		hasMoreData = true;
	}

	return hasMoreData;
}

//-----------------------------------//

void AudioSource::setSound(const SoundHandle& handle)
{
	soundHandle = handle;
	sound = handle.Resolve();

	// Empty the source if the sound is not valid.
	if( !sound )
	{
		empty();
		return;
	}

	AudioDevice* device = GetAudioDevice();

	if( !sound->getStreamed() )
	{
		buffers[0] = device->prepareBuffer(sound);
		
		if( sound->isLoaded() )
			AudioBufferSound(buffers[0].get(), sound);
	}
	else
	{
		ResourceStream& resourceStream = *sound->stream;

		AudioBufferDetails details;
		AudioGetBufferDataDetails(details, sound);

		for( size_t i = 0; i < AudioSourceNumBuffers; ++i )
		{
			buffers[i] = device->createBuffer();
			AudioBuffer* buffer = buffers[i].get();

			AudioStreamQueueBuffer(id, buffer->id, details, resourceStream, loop);
		}
	}

	update();
}

//-----------------------------------//

bool AudioSource::update()
{
	if( !sound ) return false;
	
	bool isStreamed = sound->getStreamed();
	
	if( !isStreamed ) goto out;
 
	int processed;
	alGetSourcei(id, AL_BUFFERS_PROCESSED, &processed);
	
	if( processed == 0 ) return true;

	AudioBufferDetails details;
	AudioGetBufferDataDetails(details, sound);

	ResourceStream& stream = *sound->stream;

	while(processed--)
	{
		ALuint buffer;
		alSourceUnqueueBuffers(id, 1, &buffer);
		
		if(AudioCheckError())
			LogAudio("Could not unqueue buffer from audio source");
 
		if( !AudioStreamQueueBuffer(id, buffer, details, stream, loop) )
			break;
	}

out:

	return true;
}

//-----------------------------------//

void AudioSource::play( const int count )
{
	if( !sound ) return;

	if( !sound->getStreamed() && !buffers[0]->getUploaded() )
	{
		buffers[0]->onBufferUploaded.Connect(this, &AudioSource::onBufferUploaded);
		state = AudioSource::PENDING_PLAY;

		if(state != SourceState::PAUSED && state != SourceState::PLAYING)
			alSourcei(id, AL_BUFFER, buffers[0]->getId());

		return;
	}

	alSourcePlay(id);

	state = SourceState::PLAYING;

	if(AudioCheckError())
		LogAudio("Could not play audio source");
}

//-----------------------------------//

void AudioSource::onBufferUploaded(AudioBuffer* newBuffer)
{
	assert( newBuffer == buffers[0].get() );
	
	// This gets called if the source was asked to play but if the buffer
	// was not uploaded yet. This can happen due to asynchronous nature
	// of the loading of the resources.

	if( state != AudioSource::PENDING_PLAY ) return;
	play();
}

//-----------------------------------//

void AudioSource::stop()
{
	alSourceStop(id);

	if(AudioCheckError())
		LogAudio("Could not stop audio source");

	state = AudioSource::STOPPED;

	if( sound && sound->stream )
		sound->stream->reset();
}

//-----------------------------------//

void AudioSource::pause()
{
	context->makeCurrent();

	alSourcePause(id);

	if(AudioCheckError())
		LogAudio( "Could not pause audio source");
	
	state = AudioSource::PAUSED;
}

//-----------------------------------//

bool AudioSource::isPlaying()
{
	ALenum state;
	alGetSourcei(id, AL_SOURCE_STATE, &state);

	return (state == AL_PLAYING);
}

//-----------------------------------//

bool AudioSource::isPaused()
{
	ALenum state;
	alGetSourcei(id, AL_SOURCE_STATE, &state);

	return (state == AL_PAUSED);
}

//-----------------------------------//

void AudioSource::setVolume(float volume)
{
	alSourcef(id, AL_GAIN, volume);
}

//-----------------------------------//

void AudioSource::setPitch(float pitch)
{
	alSourcef(id, AL_PITCH, pitch);
}

//-----------------------------------//

void AudioSource::setRolloff(float rolloff)
{
	alSourcef(id, AL_ROLLOFF_FACTOR, rolloff);
}

//-----------------------------------//

void AudioSource::setRolloffMode( RolloffMode::Enum mode )
{
	alDistanceModel(mode);
}

//-----------------------------------//

void AudioSource::setReferenceDistance( float distance )
{
	alSourcef(id, AL_REFERENCE_DISTANCE, distance);
}

//-----------------------------------//

void AudioSource::setMaxDistance( float distance )
{
	alSourcef(id, AL_MAX_DISTANCE, distance);
}

//-----------------------------------//

void AudioSource::setLoop(bool state)
{
	loop = state;
}

//-----------------------------------//

void AudioSource::setPosition( const Vector3& pos )
{
	alSource3f(id, AL_POSITION, pos.x, pos.y, pos.z);

	if(AudioCheckError())
		LogAudio( "Could not set position in audio source");
}

//-----------------------------------//

NAMESPACE_ENGINE_END

#endif