/************************************************************************
*
* vaporEngine (2008-2009)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#include "vapor/PCH.h"

#ifdef VAPOR_SHADER_GLSL

#include "vapor/resources/GLSL_Loader.h"

using vapor::vfs::File;

//-----------------------------------//

namespace vapor {
	namespace resources {

//-----------------------------------//

GLSL_Loader::GLSL_Loader()
{
	extensions.push_back( "vp" );
	extensions.push_back( "fp" );
	extensions.push_back( "gp" );
}

//-----------------------------------//

Shader* GLSL_Loader::decode(File& file)
{
	std::vector<byte> text = file.read();

	return nullptr;
}

//-----------------------------------//

} } // end namespaces

#endif