/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "vapor/scene/Geometry.h"

namespace vapor { namespace scene {

//-----------------------------------//

/**
 * Implements a skydome, a higher-quality alternative to skyboxes, using
 * half of a sphere in the background (can also be 3/4 or more if needed).
 * This component is tipically used to simulate a sky system, with moving
 * clouds, and a big texture representing the horizon, but this class only
 * takes care of the sphere generation and texture mapping. A proper sky
 * system can be implemented on top of this class if needed.
 */

class Skydome : public scene::Geometry
{
public:

	Skydome( render::MaterialPtr mat );
	virtual ~Skydome();

	// Returns the name of this component.
	virtual const std::string& getType() const;

protected:

	// Returns a vertex buffer with the sphere geometry.
	render::VertexBufferPtr generateSphere();

	static const std::string& type;
};

//-----------------------------------//

TYPEDEF_SHARED_POINTER_FROM_CLASS( Skydome );

//-----------------------------------//

} } // end namespaces