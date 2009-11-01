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

#include "vapor/render/GLSL_Shader.h"

using vapor::vfs::File;

namespace vapor {
	namespace resources {

//-----------------------------------//

GLSL_Loader::GLSL_Loader()
{
	extensions.push_back( "vs" );
	extensions.push_back( "fs" );
	extensions.push_back( "gs" );
}

//-----------------------------------//

Shader* GLSL_Loader::decode(const File& file)
{
	std::vector<byte> text = file.read();

	const std::string& path = file.getPath();

	// check if it has a file extension
	uint ch = path.find_last_of( "." );
	
	if( ch == std::string::npos ) 
	{
		return nullptr;
	}

	// get the file extension
	std::string ext = path.substr( ++ch );

	ShaderType::Enum type;

	if( ext == "vs" )
		type = ShaderType::Vertex;
	else if( ext == "fs" )
		type = ShaderType::Fragment;
	else if( ext == "gs" )
		type = ShaderType::Geometry;
	else
		return nullptr;

	std::string str( text.begin(), text.end() );

	render::GLSL_Shader* shader;
	shader = new render::GLSL_Shader( type, str );
	shader->setURI( path );

	return shader;
}

//-----------------------------------//

} } // end namespaces

#endif