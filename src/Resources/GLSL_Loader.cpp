/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Resources/API.h"

#ifdef ENABLE_RENDERER_OPENGL_GLSL

#include "Resources/GLSL_Loader.h"
#include "Resources/GLSL_Shader.h"

NAMESPACE_RESOURCES_BEGIN

//-----------------------------------//

REFLECT_CHILD_CLASS(GLSL_Loader, ResourceLoader)
REFLECT_CLASS_END()

//-----------------------------------//

GLSL_Loader::GLSL_Loader()
{
	extensions.push_back( "glsl" );
}

//-----------------------------------//

Resource* GLSL_Loader::prepare(ResourceLoadOptions&)
{
	GLSL_Shader* shader = Allocate(GetResourcesAllocator(), GLSL_Shader);
	return shader;
}

//-----------------------------------//

bool GLSL_Loader::decode(ResourceLoadOptions& options)
{
	String text;
	StreamReadString(options.stream, text);

	GLSL_Shader* shader = static_cast<GLSL_Shader*>(options.resource);
	shader->setText( text );
	shader->parse();

	return true;
}

//-----------------------------------//

NAMESPACE_RESOURCES_END

#endif