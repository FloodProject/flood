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
#include "vapor/resources/Sound.h"

namespace vapor {
	namespace scene {

/**
 * Uses the position and orientation defined in the node 
 * as the sound source location.
 */

class Sound : public Transformable
{
public:

	void setResource( shared_ptr<resources::Sound> resource );
  
	/* will this update current playing sounds? yes*/
	void setVolume( float volume );

	void play(int count = 1);

	void stop();

	void pause();

	void rewind();

	void setPitch( float pitch );

	void setRollOff( float rollOff );
  
private:
	
	shared_ptr<resources::Sound> resource;
};

} } // end namespaces