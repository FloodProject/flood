/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "vapor/scene/Geometry.h"

FWD_DECL_INTRUSIVE(Mesh)

namespace vapor {

//-----------------------------------//

/**
 * Models are specific instances of meshes in the scene. This way the
 * engine can share the mesh geometry and render the same model at
 * lots of places in the scene. This takes a mesh resource and waits
 * for it to be fully loaded.
 */

class VAPOR_API Model : public Geometry
{
	DECLARE_CLASS_()

public:

	Model();
	Model( const MeshPtr& mesh );

	// Builds the mesh when it is fully loaded.
	void update( double delta );
  
protected:

	// Mesh that the model renders.
	MeshPtr mesh;

	// Has the model been built.
	bool modelBuilt;
};

TYPEDEF_SHARED_POINTER_FROM_TYPE( Model );

//-----------------------------------//

} // end namespace