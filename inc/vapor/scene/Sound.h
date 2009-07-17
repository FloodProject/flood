/************************************************************************
*
* vaporEngine (2008-2009)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#pragma once

#include "vapor/Platform.h"
#include "vapor/scene/Transform.h"
#include "vapor/audio/Source.h"
#include "vapor/resources/Sound.h"

namespace vapor {
	namespace scene {

/**
 * Uses the position and orientation defined in the node 
 * as the sound source location.
 */

class Sound : public Transformable, public audio::Source
{
public:

	Sound(shared_ptr<Listener> ls, shared_ptr<resources::Sound> sound);
	virtual ~Sound();

	virtual std::string save(int indent = 0);

protected:

	// Hold a shared_ptr so the Listener is only deleted when
	// no other sources exist. We need the Listener to switch
	// the context.
	shared_ptr<Listener> ls;
};

} } // end namespaces