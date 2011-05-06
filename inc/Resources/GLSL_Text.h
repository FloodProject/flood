/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "Resources/Text.h"

namespace vapor {

//-----------------------------------//

/**
 * GLSL resources are text files which contain all the sub-shaders of
 * a GLSL program. They contain various blocks with the source code of
 * vertex and pixel shaders.
 */

REFLECT_DECLARE_CLASS(GLSL_Text)

class RESOURCE_API GLSL_Text : public Text
{
	REFLECT_DECLARE_OBJECT(GLSL_Text)

public:

	// Returns the vertex shader source code.
	GETTER(VertexSource, const String&, vertex)

	// Returns the fragment shader source code.
	GETTER(FragmentSource, const String&, pixel)

	// Returns the proper resource group for the shader.
	GETTER(ResourceGroup, ResourceGroup::Enum, ResourceGroup::Shaders)

	// Parses the text into shaders.
	void parse();

protected:

	String vertex;
	String pixel;
};

TYPEDEF_RESOURCE_HANDLE_FROM_TYPE( GLSL_Text );

//-----------------------------------//

} // end namespace

//#endif