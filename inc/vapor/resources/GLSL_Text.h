/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "vapor/resources/Text.h"

namespace vapor { namespace resources {

//-----------------------------------//

/**
 * GLSL resources are text files which contain all the sub-shaders of
 * a GLSL program. They contain various blocks with thesource code of
 * vertex and pixel shaders.
 */

class VAPOR_API GLSL_Text : public Text
{
public:

	// Returns the vertex shader source code.
	IMPLEMENT_GETTER(VertexSource, const std::string&, vertex)

	// Returns the fragment shader source code.
	IMPLEMENT_GETTER(FragmentSource, const std::string&, pixel)

	// Returns the geometry shader source code.
	//IMPLEMENT_GETTER(GeometrySource, const std::string&, geometry)

	// Returns the proper resource group for the shader.
	IMPLEMENT_GETTER(ResourceGroup, ResourceGroup::Enum, ResourceGroup::Shaders)

	void parse();

protected:

	std::string vertex;
	std::string pixel;
};

TYPEDEF_RESOURCE_POINTER_FROM_TYPE( GLSL_Text );

//-----------------------------------//

} } // end namespaces

//#endif