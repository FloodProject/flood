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

using std::static_pointer_cast;

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

void Listener::update( float delta )
{

}

//-----------------------------------//

std::shared_ptr<audio::Context> Listener::getContext(ListenerPtr ls)
{
	std::shared_ptr<audio::Context> ctx = 
		std::static_pointer_cast<audio::Context>(ls);

	return ctx;
}


//-----------------------------------//

} } // end namespaces

#endif