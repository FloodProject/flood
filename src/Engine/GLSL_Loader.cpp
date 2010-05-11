/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "vapor/PCH.h"

#ifdef VAPOR_SHADER_GLSL

#include "vapor/resources/GLSL_Loader.h"
#include "vapor/resources/GLSL_Text.h"

namespace vapor { namespace resources {

using vapor::vfs::File;

//-----------------------------------//

GLSL_Loader::GLSL_Loader()
{
	extensions.push_back( "glsl" );
}

//-----------------------------------//

Resource* GLSL_Loader::prepare(const File& file)
{
	GLSL_Text* shader = new GLSL_Text;
	return shader;
}

//-----------------------------------//

bool GLSL_Loader::decode(const File& file, Resource* res)
{
	std::vector<byte> text = file.read();
	std::string str( text.begin(), text.end() );

	GLSL_Text* shader = static_cast<GLSL_Text*>(res);
	shader->setText( str );
	shader->parse();

	return true;
}

//-----------------------------------//

} } // end namespaces

#endif