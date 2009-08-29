/************************************************************************
*
* vaporEngine (2008-2009)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#include "vapor/PCH.h"

#ifdef VAPOR_AUDIO_OPENAL

#include "vapor/scene/Listener.h"

namespace vapor {
	namespace scene {

using std::tr1::static_pointer_cast;

//-----------------------------------//

Listener::Listener(audio::Device* device)
	: Context(device)
{

}

//-----------------------------------//

Listener::~Listener()
{

}

//-----------------------------------//

shared_ptr<audio::Context> Listener::getContext(shared_ptr<Listener> ls)
{
	shared_ptr<audio::Context> ctx = 
		static_pointer_cast<audio::Context>(ls);

	return ctx;
}


//-----------------------------------//

} } // end namespaces

#endif