/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "vapor/PCH.h"

#ifdef VAPOR_AUDIO_OPENAL

#include "scene/Source.h"
#include "audio/Source.h"
#include "resources/Sound.h"
#include "Engine.h"

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
	FIELD_ENUM_SETTER(Source, SourceState, state, State)
	FIELD_ENUM_SETTER(Source, SourceMode, mode, Mode)
	FIELD_PRIMITIVE_SETTER(Source, float, volume, Volume)
	FIELD_PRIMITIVE_SETTER(Source, float, pitch, Pitch)
	FIELD_PRIMITIVE_SETTER(Source, float, rollOff, RollOff)
	FIELD_PRIMITIVE_SETTER(Source, bool, loop, Loop)
	FIELD_CLASS_PTR_SETTER(Source, Sound, sound, Sound)
END_CLASS()

//-----------------------------------//

Source::Source()
	: state(SourceState::Stop)
	, mode(SourceMode::Static)
	, volume(0.75f)
	, pitch(1.0f)
	, rollOff(1.0f)
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

	if(audioSource)
		audioSource->setVolume(volume);
}

//-----------------------------------//

void Source::setPitch(float pitch)
{
	this->pitch = pitch;

	if(audioSource)
		audioSource->setPitch(pitch);
}

//-----------------------------------//

void Source::setRollOff(float rollOff)
{
	this->rollOff = rollOff;

	if(audioSource)
		audioSource->setRollOff(rollOff);
}

//-----------------------------------//

void Source::setLoop(bool state)
{
	this->loop = state;

	if(audioSource)
		audioSource->setLoop(state);
}

//-----------------------------------//

void Source::setState( SourceState::Enum state )
{
	this->state = state;

	if(!audioSource)
		return;

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

	if(!audioSource)
		return;
}

//-----------------------------------//

void Source::setSound( const SoundPtr& sound )
{
	this->sound = sound;

	AudioDevice* audioDevice = GetEngine()->getAudioDevice();
	AudioContext* audioContext = audioDevice->getMainContext();

	audioSource = new AudioSource(audioContext, sound);

	setVolume(volume);
	setPitch(pitch);
	setRollOff(rollOff);
	setMode(mode);
	setState(state);
}

//-----------------------------------//

void Source::update( double delta )
{
	if( !audioSource )
		return;

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