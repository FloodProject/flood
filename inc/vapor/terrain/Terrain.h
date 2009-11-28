/************************************************************************
*
* vaporEngine (2008-2009)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#pragma once

#include "vapor/Platform.h"
#include "vapor/math/Vector3.h"

namespace vapor { namespace scene {

//-----------------------------------//

/**
 * Simple terrain implementation. A terrain is a piece of geometry that is
 * represented by a heightmap, that is, a small bitmap that has the height 
 * of the terrain in a coordinate.
 */

class VAPOR_API Terrain : public Component
{
public:

	Terrain();

	// Updates this component.
	virtual void update( float delta );

protected:

	static const std::string& type;
};

//-----------------------------------//

TYPEDEF_SHARED_POINTER_FROM_CLASS( Terrain );

//-----------------------------------//

} } // end namespaces
