/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Resources/API.h"
#include "Resources/GLSL_Text.h"
#include "Core/Log.h"

namespace vapor {

//-----------------------------------//

REFLECT_CHILD_CLASS(GLSL_Text, Text)
REFLECT_CLASS_END()

//-----------------------------------//

static const String VertexBlock("[vertex]");
static const String FragmentBlock("[fragment]");

void GLSL_Text::parse()
{
	// Get the vertex and pixel shader block indicies.
	size_t v = text.find(VertexBlock);
	size_t p = text.find(FragmentBlock);

	if( v == String::npos )
	{
		LogWarn( "No vertex shader found in '%s'", path.c_str() );
		vertex.clear();
	}
	else
	{
		vertex = text.substr(v+VertexBlock.size(), p-v-FragmentBlock.size());
	}

	if( p == String::npos )
	{
		LogWarn( "No fragment shader found in '%s'", path.c_str() );
		pixel.clear();
	}
	else
	{
		pixel = text.substr(p+FragmentBlock.size(), String::npos);
	}
}

//-----------------------------------//

} // end namespace