/************************************************************************
*
* vaporEngine (2008-2009)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#include "vapor/CompileOptions.h"

#ifdef VAPOR_AUDIO_OPENAL

#include "vapor/scene/Sound.h"

namespace vapor {
	namespace scene {

//-----------------------------------//

Sound::Sound( shared_ptr<Listener> ls, shared_ptr<resources::Sound> sound )
	: Source(Listener::getContext(ls), sound)
{

}

//-----------------------------------//

Sound::~Sound()
{

}

//-----------------------------------//

std::string Sound::save(int indent)
{
	return "";
}

//-----------------------------------//

} } // end namespaces

#endif