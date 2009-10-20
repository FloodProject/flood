/************************************************************************
*
* vaporEngine (2008-2009)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#pragma once

#include "vapor/Platform.h"
#include "vapor/scene/Geometry.h"

namespace vapor {
	namespace scene {

//-----------------------------------//

/**
 * Simple grid scene node. Can be used for editor applications.
 */

class Grid : public scene::Geometry
{
public:

	Grid( render::MaterialPtr mat );
	virtual ~Grid();

private:

	// Returns a vertex buffer with the grid geometry.
	render::VertexBufferPtr getGridBuffer();

	// Grid size in the X and Z axis.
	float sizeX, sizeZ;

	// Grid divisions in the X and Z axis.
	int divX, divZ;

	// Makes the main lines more visible. 
	bool strongMainLines;
};

//-----------------------------------//

TYPEDEF_SHARED_POINTER_FROM_CLASS( Grid );

//-----------------------------------//

} } // end namespaces