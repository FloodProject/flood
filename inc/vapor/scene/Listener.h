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

namespace vapor {
	namespace scene {

class Listener : public Transformable
{
public:

  Listener();
  
  void setVolume( float volume );
  
  void makeCurrent();
};


} } // end namespaces