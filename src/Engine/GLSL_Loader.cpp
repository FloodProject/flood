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
#include "vapor/render/GLSL_Shader.h"

using vapor::vfs::File;
using namespace vapor::render;

namespace vapor { namespace resources {

//-----------------------------------//

GLSL_Loader::GLSL_Loader()
{
	extensions.push_back( "vs" );
	extensions.push_back( "fs" );
	extensions.push_back( "gs" );
}

//-----------------------------------//

Resource* GLSL_Loader::prepare(const File& file)
{
	GLSL_Shader* shader = new GLSL_Shader();

	std::string ext = file.getExtension();

	ShaderType::Enum type;
	if( ext == "vs" )
		type = ShaderType::Vertex;
	else if( ext == "fs" )
		type = ShaderType::Fragment;
	else if( ext == "gs" )
		type = ShaderType::Geometry;
	else
		assert( false ); // This should not be reachable
	
	shader->setType( type );

	return shader;
}

//-----------------------------------//

bool GLSL_Loader::decode(const File& file, Resource* res)
{
	std::vector<byte> text = file.read();
	std::string str( text.begin(), text.end() );

	GLSL_Shader* shader = static_cast<GLSL_Shader*>( res );
	shader->setText( str );

	return true;
}

//-----------------------------------//

} } // end namespaces

#endif