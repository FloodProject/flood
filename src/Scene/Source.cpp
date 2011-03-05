/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "vapor/PCH.h"

#ifdef VAPOR_AUDIO_OPENAL

#include "Scene/Source.h"
#include "Audio/Source.h"
#include "Resources/Sound.h"

namespace vapor {

//-----------------------------------//

BEGIN_ENUM(SourceState)
	ENUM(Play)
	ENUM(Pause)
	ENUM(Stop)
END_ENUM()

BEGIN_ENUM(SourceMode)
	ENUM(Static)
	ENUM(Streaming)
END_ENUM()

BEGIN_CLASS_PARENT(Source, Component)
	FIELD_ENUM_SETTER(SourceState, state, State)
	FIELD_ENUM_SETTER(SourceMode, mode, Mode)
	FIELD_PRIMITIVE_SETTER(bool, loop, Loop)
	FIELD_PRIMITIVE_SETTER(float, volume, Volume)
	FIELD_PRIMITIVE_SETTER(float, pitch, Pitch)
	FIELD_PRIMITIVE_SETTER(float, minDistance, MinDistance)
	FIELD_PRIMITIVE_SETTER(float, maxDistance, MaxDistance)
	FIELD_ENUM_SETTER(RolloffMode, rolloffMode, RolloffMode)
	FIELD_PRIMITIVE_SETTER(float, rolloff, Rolloff)
	FIELD_CLASS_PTR_SETTER(Sound, SoundPtr, sound, RefPointer, Sound)
END_CLASS()

//-----------------------------------//

Source::Source()
	: state(SourceState::Stop)
	, mode(SourceMode::Static)
	, volume(1.0f)
	, pitch(1.0f)
	, minDistance(20.0f)
	, maxDistance(40.0f)
	, rolloffMode(RolloffMode::Logarithmic)
	, rolloff(1.0f)
	, loop(false)
	, sound(nullptr)
	, audioSource(nullptr)
{ }

//-----------------------------------//

Source::~Source()
{
	delete audioSource;
}

//-----------------------------------//

void Source::setVolume(float volume)
{
	this->volume = volume;
	if(audioSource) audioSource->setVolume(volume);
}

//-----------------------------------//

void Source::setPitch(float pitch)
{
	this->pitch = pitch;
	if(audioSource) audioSource->setPitch(pitch);
}

//-----------------------------------//

void Source::setMinDistance( float distance )
{
	this->minDistance = distance;
	if(audioSource) audioSource->setReferenceDistance(distance);
}

//-----------------------------------//

void Source::setMaxDistance( float distance )
{
	this->maxDistance = distance;
	if(audioSource) audioSource->setMaxDistance(distance);
}

//-----------------------------------//

void Source::setRolloff(float rolloff)
{
	this->rolloff = rolloff;
	if(audioSource) audioSource->setRolloff(rolloff);
}

//-----------------------------------//

void Source::setRolloffMode(RolloffMode::Enum mode)
{
	this->rolloffMode = mode;
	if(audioSource) audioSource->setRolloffMode(rolloffMode);
}

//-----------------------------------//

void Source::setLoop(bool state)
{
	this->loop = state;
	if(audioSource) audioSource->setLoop(state);
}

//-----------------------------------//

void Source::setState( SourceState::Enum state )
{
	this->state = state;
	
	if(!audioSource) return;

	if(state == SourceState::Play && !audioSource->isPlaying())
		audioSource->play();
	
	else if(state == SourceState::Pause)
		audioSource->pause();
	
	else if(state == SourceState::Stop)
		audioSource->stop();
}

//-----------------------------------//

void Source::setMode( SourceMode::Enum mode )
{
	this->mode = mode;
	if(!audioSource) return;
}

//-----------------------------------//

void Source::setSound( const SoundPtr& sound )
{
	this->sound = sound;

	AudioDevice* device = GetAudioDevice();
	AudioContext* context = device->getMainContext();

	audioSource = new AudioSource(context, sound);

	setVolume(volume);
	setPitch(pitch);
	setRolloff(rolloff);
	setMode(mode);
	setState(state);
}

//-----------------------------------//

void Source::update( float delta )
{
	if( !audioSource ) return;

	if( audioSource->isPlaying() )
		state = SourceState::Play;
	
	else if( audioSource->isPaused() )
		state = SourceState::Pause;
	
	else
		state = SourceState::Stop;
}

//-----------------------------------//

} // end namespace

#endif