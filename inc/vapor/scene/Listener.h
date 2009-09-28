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
#include "vapor/audio/Context.h"
#include "vapor/scene/Transform.h"

namespace vapor {
	namespace scene {

//-----------------------------------//

class VAPOR_API Listener : public Transformable, public audio::Context
{
public:

	Listener(audio::Device* device);
	virtual ~Listener();

	virtual void update( double delta );

	virtual const std::string save(int indent = 0) { return ""; }

	static shared_ptr<audio::Context> getContext(shared_ptr<Listener> ls);
};

//-----------------------------------//

typedef tr1::shared_ptr< Listener > ListenerPtr;

//-----------------------------------//

} } // end namespaces

#endif