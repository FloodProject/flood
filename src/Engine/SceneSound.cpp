/************************************************************************
*
* vaporEngine (2008-2009)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#include "vapor/scene/Sound.h"
#include "vapor/audio/Device.h"
#include "vapor/resources/Sound.h"

using namespace vapor::resources;

namespace vapor {
	namespace scene {

//-----------------------------------//

Sound::Sound(shared_ptr<Resource> sound)
	: resource(sound) 
{
}

//-----------------------------------//


//-----------------------------------//

} } // end namespaces