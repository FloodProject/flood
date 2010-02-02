/************************************************************************
*
* vaporEngine (2008-2010)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#pragma once

#ifdef VAPOR_AUDIO_OPENAL

#include "vapor/scene/Transform.h"
#include "vapor/scene/Listener.h"

#include "vapor/audio/Source.h"
#include "vapor/resources/Sound.h"

namespace vapor { namespace scene {

//-----------------------------------//

/**
 * Uses the position and orientation defined in the node 
 * as the sound source location.
 */

class VAPOR_API Sound : public Transform, public audio::Source
{
public:

	Sound(ListenerPtr ls, resources::SoundPtr sound);
	virtual ~Sound();

	virtual void update( double delta );

	virtual const std::string save(int indent = 0);
};

//-----------------------------------//

TYPEDEF_SHARED_POINTER_FROM_CLASS( Sound );

//-----------------------------------//

} } // end namespaces

#endif