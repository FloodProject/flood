/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "Scene/Geometry.h"

NAMESPACE_ENGINE_BEGIN

//-----------------------------------//

/**
 * Simple grid component. Can be used for editor applications.
 */

REFLECT_DECLARE_CLASS(Grid)

class API_ENGINE Grid : public Geometry
{
	REFLECT_DECLARE_OBJECT(Grid)

public:

	Grid();

	void update( float update ) OVERRIDE;

protected:

	// Returns a vertex buffer with the grid geometry.
	GeometryBufferPtr buildGeometry();

	// Grid size in the X and Z axis.
	float sizeX, sizeZ;

	// Grid divisions in the X and Z axis.
	int32 divX, divZ;

	// Makes the main lines more visible. 
	bool strongMainLines;
};

TYPEDEF_INTRUSIVE_POINTER_FROM_TYPE( Grid );

//-----------------------------------//

NAMESPACE_ENGINE_END