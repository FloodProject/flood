/************************************************************************
*
* vaporEngine (2008-2009)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#include "vapor/PCH.h"

#ifdef VAPOR_AUDIO_OPENAL

#include "vapor/scene/Sound.h"

namespace vapor {
	namespace scene {

//-----------------------------------//

Sound::Sound( ListenerPtr ls, resources::SoundPtr sound )
	: Source( Listener::getContext( ls ), sound )
{

}

//-----------------------------------//

Sound::~Sound()
{

}

//-----------------------------------//

void Sound::update()
{

}

//-----------------------------------//

const std::string Sound::save(int indent)
{
	return "";
}

//-----------------------------------//

} } // end namespaces

#endif