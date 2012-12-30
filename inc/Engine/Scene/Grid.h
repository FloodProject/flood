/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

#include "Engine/Scene/Geometry.h"

NAMESPACE_ENGINE_BEGIN

//-----------------------------------//

/**
 * Simple grid component. Can be used for editor applications.
 */

API_SCENE REFLECT_DECLARE_CLASS(Grid)

class API_SCENE Grid : public Geometry
{
	REFLECT_DECLARE_OBJECT(Grid)

public:

	Grid();

	virtual void update( float update ) OVERRIDE;

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