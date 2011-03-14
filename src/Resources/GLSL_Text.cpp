/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Resources/API.h"
#include "Resources/GLSL_Text.h"
#include "Log.h"

namespace vapor {

//-----------------------------------//

BEGIN_CLASS_PARENT(GLSL_Text, Text)
END_CLASS()

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
		LogWarn( "No vertex shader found in '%s'", path.c_str() );
		vertex.clear();
	}
	else
	{
		vertex = text.substr(v+VertexBlock.size(), p-v-FragmentBlock.size());
	}

	if( p == std::string::npos )
	{
		LogWarn( "No fragment shader found in '%s'", path.c_str() );
		pixel.clear();
	}
	else
	{
		pixel = text.substr(p+FragmentBlock.size(), std::string::npos);
	}
}

//-----------------------------------//

} // end namespace