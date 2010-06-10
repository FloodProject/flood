/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#ifdef VAPOR_AUDIO_OPENAL

#include "vapor/scene/Transform.h"
#include "vapor/scene/Listener.h"

#include "vapor/audio/Source.h"
#include "vapor/resources/Sound.h"

namespace vapor {

//-----------------------------------//

/**
 * Uses the position and orientation defined in the node 
 * as the sound source location.
 */

class VAPOR_API Sound : public audio::Source, public Transform, 
{
public:

	Sound(ListenerPtr ls, SoundPtr sound);
	virtual ~Sound() { }

	virtual void update( double delta );

	virtual const std::string save(int indent = 0);
};

TYPEDEF_INTRUSIVE_POINTER_FROM_TYPE( Sound );

//-----------------------------------//

} // end namespace

#endif