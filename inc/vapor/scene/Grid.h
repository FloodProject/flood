/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "vapor/scene/Geometry.h"

namespace vapor {

//-----------------------------------//

/**
 * Simple grid component. Can be used for editor applications.
 */

class VAPOR_API Grid : public Geometry
{
public:

	Grid();

	// Returns the type of this component.
	GETTER(Type, const std::string&, Grid::type)

protected:

	// Returns a vertex buffer with the grid geometry.
	VertexBufferPtr buildGeometry();

	// Grid size in the X and Z axis.
	float sizeX, sizeZ;

	// Grid divisions in the X and Z axis.
	int divX, divZ;

	// Makes the main lines more visible. 
	bool strongMainLines;

	static const std::string& type;
};

//-----------------------------------//

TYPEDEF_SHARED_POINTER_FROM_TYPE( Grid );

//-----------------------------------//

} // end namespace