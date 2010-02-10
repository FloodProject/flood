/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#ifdef VAPOR_AUDIO_OPENAL

#include "vapor/audio/Context.h"
#include "vapor/scene/Transform.h"

namespace vapor { namespace scene {

//-----------------------------------//

class VAPOR_API Listener : public Transform, public audio::Context
{
public:

	Listener(audio::Device* device);
	virtual ~Listener();

	virtual void update( double delta );

	virtual const std::string save(int UNUSED(indent) = 0) { return ""; }

	static std::shared_ptr<audio::Context> getContext(std::shared_ptr<Listener> ls);
};

//-----------------------------------//

TYPEDEF_SHARED_POINTER_FROM_CLASS( Listener );

//-----------------------------------//

} } // end namespaces

#endif