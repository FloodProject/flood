/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "vapor/PCH.h"
#include "vapor/resources/GLSL_Text.h"

namespace vapor {

//-----------------------------------//

static const std::string VertexBlock("[vertex]");
static const std::string FragmentBlock("[fragment]");

void GLSL_Text::parse()
{
	// Get the vertex and pixel shader block indicies.
	size_t v = text.find(VertexBlock);
	size_t p = text.find(FragmentBlock);

	if( v == std::string::npos )
	{
		warn( "glsl", "No vertex shader found in '%s'", uri.c_str() );
		vertex.clear();
	}
	else
	{
		vertex = text.substr(v+VertexBlock.size(), p-v-FragmentBlock.size());
	}

	if( p == std::string::npos )
	{
		warn( "glsl", "No fragment shader found in '%s'", uri.c_str() );
		pixel.clear();
	}
	else
	{
		pixel = text.substr(p+FragmentBlock.size(), std::string::npos);
	}
}

//-----------------------------------//

} // end namespace