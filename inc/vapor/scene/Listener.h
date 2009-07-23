/************************************************************************
*
* vaporEngine (2008-2009)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#pragma once

#include "vapor/Platform.h"
#include "vapor/audio/Context.h"
#include "vapor/scene/Transform.h"

namespace vapor {
	namespace scene {

class Listener : public Transformable, public audio::Context
{
public:

	Listener(audio::Device* device);
	virtual ~Listener();

	virtual std::string save(int indent = 0) { return ""; }

	static shared_ptr<audio::Context> getContext(shared_ptr<Listener> ls);
};

} } // end namespaces