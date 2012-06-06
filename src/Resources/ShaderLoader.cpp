/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Resources/API.h"

#ifdef ENABLE_RENDERER_OPENGL_GLSL

#include "Resources/ShaderLoader.h"
#include "Resources/ShaderMaterial.h"
#include "Core/Log.h"

NAMESPACE_RESOURCES_BEGIN

//-----------------------------------//

class ShaderTextMaterial : public ShaderMaterial
{
public:

	ShaderTextMaterial() {}

	const char* getVertexSource() const OVERRIDE { return vertex.c_str(); }
	const char* getFragmentSource() const OVERRIDE { return fragment.c_str(); }

	String vertex;
	String fragment;
};

//-----------------------------------//

REFLECT_CHILD_CLASS(ShaderLoader, ResourceLoader)
REFLECT_CLASS_END()

//-----------------------------------//

ShaderLoader::ShaderLoader()
{
	extensions.push_back( "glsl" );
}

//-----------------------------------//

Resource* ShaderLoader::prepare(ResourceLoadOptions&)
{
	ShaderMaterial* shader = AllocateHeap(ShaderTextMaterial);
	return shader;
}

//-----------------------------------//

static const String gs_VertexBlock("[vertex]");
static const String gs_FragmentBlock("[fragment]");

bool ShaderLoader::decode(ResourceLoadOptions& options)
{
	String text;
	StreamReadString(options.stream, text);

	ShaderTextMaterial* shader = static_cast<ShaderTextMaterial*>(options.resource);

	// Get the vertex and pixel shader block indicies.
	size_t offsetVertex = text.find(gs_VertexBlock);
	size_t offsetFragment = text.find(gs_FragmentBlock);

	if( offsetVertex == String::npos )
		LogWarn( "No vertex shader found in '%s'", shader->path.c_str() );
	else
		shader->vertex = text.substr(offsetVertex + gs_VertexBlock.size(),
			offsetFragment - offsetVertex - gs_FragmentBlock.size());

	if( offsetFragment == String::npos )
		LogWarn( "No fragment shader found in '%s'", shader->path.c_str() );
	else
		shader->fragment = text.substr(offsetFragment + gs_FragmentBlock.size(), String::npos);

	return true;
}

//-----------------------------------//

NAMESPACE_RESOURCES_END

#endif