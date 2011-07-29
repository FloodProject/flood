/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Resources/API.h"

#ifdef ENABLE_SHADER_GLSL

#include "Resources/GLSL_Loader.h"
#include "Resources/GLSL_Text.h"

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

Resource* GLSL_Loader::prepare(const Stream& stream)
{
	GLSL_Text* shader = Allocate(GLSL_Text, GetResourcesAllocator());
	return shader;
}

//-----------------------------------//

bool GLSL_Loader::decode(const Stream& stream, Resource* res)
{
	String text;
	StreamReadString((Stream*) &stream, text);

	GLSL_Text* shader = static_cast<GLSL_Text*>(res);
	shader->setText( text );
	shader->parse();

	return true;
}

//-----------------------------------//

NAMESPACE_RESOURCES_END

#endif