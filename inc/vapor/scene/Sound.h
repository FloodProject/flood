/************************************************************************
*
* vaporEngine (2008-2009)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#pragma once

#include "vapor/Platform.h"
#include "vapor/scene/Node.h"
#include "vapor/scene/Transform.h"
#include "vapor/resources/Resource.h"

namespace vapor {
	namespace scene {

/**
 * Uses the position and orientation defined in the node 
 * as the sound source location.
 */

class Sound : public Transformable
{
public:

	Sound( shared_ptr<resources::Resource> sound );

	void setResource( shared_ptr<resources::Resource> sound );

	void play(int count = 1);

	void stop();

	void pause();

	/* will this update current playing sounds? yes*/
	void setVolume( float volume );

	void setPitch( float pitch );

	void setRollOff( float rollOff );

	virtual std::string save() { return ""; }
  
private:
	
	shared_ptr<resources::Resource> resource;
};

} } // end namespaces