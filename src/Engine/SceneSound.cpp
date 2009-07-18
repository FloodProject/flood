/************************************************************************
*
* vaporEngine (2008-2009)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#include "vapor/scene/Sound.h"

namespace vapor {
	namespace scene {

//-----------------------------------//

Sound::Sound( shared_ptr<Listener> ls, shared_ptr<resources::Sound> sound )
	: Source(ls.get(), sound), ls(ls)
{

}

//-----------------------------------//

Sound::~Sound()
{
	//~audio::Source();
}

//-----------------------------------//

std::string Sound::save(int indent)
{
	return "";
}

//-----------------------------------//

} } // end namespaces