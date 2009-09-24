/************************************************************************
*
* vaporEngine (2008-2009)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#pragma once

#include "vapor/CompileOptions.h"

#ifdef VAPOR_AUDIO_OPENAL

#include "vapor/Platform.h"

#include "vapor/scene/Transform.h"
#include "vapor/scene/Listener.h"

#include "vapor/audio/Source.h"

#include "vapor/resources/Sound.h"

namespace vapor {
	namespace scene {

//-----------------------------------//

/**
 * Uses the position and orientation defined in the node 
 * as the sound source location.
 */

class VAPOR_API Sound : public Transformable, public audio::Source
{
public:

	Sound(ListenerPtr ls, resources::SoundPtr sound);
	virtual ~Sound();

	virtual void update( );

	virtual const std::string save(int indent = 0);
};

//-----------------------------------//

typedef tr1::shared_ptr< scene::Sound > SoundPtr;

//-----------------------------------//

} } // end namespaces

#endif