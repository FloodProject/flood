/************************************************************************
*
* vaporEngine (2008-2009)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#include "vapor/scene/Listener.h"

namespace vapor {
	namespace scene {

//-----------------------------------//

Listener::Listener(audio::Device* device)
	: context(nullptr)
{
	context = alcCreateContext(device->device, nullptr);
}

//-----------------------------------//

void Listener::makeCurrent()
{
	
}

//-----------------------------------//

} } // end namespaces