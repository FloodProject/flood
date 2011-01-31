/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Resources/API.h"

#ifdef VAPOR_SHADER_GLSL

#include "Resources/GLSL_Loader.h"
#include "Resources/GLSL_Text.h"

namespace vapor {

//-----------------------------------//

BEGIN_CLASS_PARENT(GLSL_Loader, ResourceLoader)
END_CLASS()

//-----------------------------------//

GLSL_Loader::GLSL_Loader()
{
	extensions.push_back( "glsl" );
}

//-----------------------------------//

Resource* GLSL_Loader::prepare(const Stream& stream)
{
	GLSL_Text* shader = new GLSL_Text;
	return shader;
}

//-----------------------------------//

bool GLSL_Loader::decode(const Stream& file, Resource* res)
{
	std::string text;
	file.read(text);

	GLSL_Text* shader = static_cast<GLSL_Text*>(res);
	shader->setText( text );
	shader->parse();

	return true;
}

//-----------------------------------//

} // end namespace

#endif