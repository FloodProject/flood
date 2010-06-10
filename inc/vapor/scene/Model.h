/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "vapor/scene/Geometry.h"
#include "vapor/resources/Mesh.h"

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
public:

	Model( MeshPtr mesh );

	// Builds the mesh when it is fully loaded.
	void update( double delta );

	// Gets the type of this component.
	IMPLEMENT_GETTER(Type, const std::string&, type)
  
private:

	MeshPtr mesh;

	// Component type
	static const std::string& type;
};

TYPEDEF_SHARED_POINTER_FROM_TYPE( Model );

//-----------------------------------//

} // end namespace