/************************************************************************
*
* vaporEngine (2008-2009)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#pragma once

#include "vapor/Platform.h"

#include "vapor/render/Program.h"
#include "vapor/render/Texture.h"
#include "vapor/resources/Shader.h"

namespace vapor {
	namespace render {

//-----------------------------------//

/**
 * Rendering material.
 */

class VAPOR_API Material
{
public:

	Material( const std::string& name, ProgramPtr program );
	~Material();

	// Gets the textual name of the material.
	const std::string& getName() const;

	// Adds a texture to the material.
	void addTexture( TexturePtr tex );

	// Gets the associated program.
	ProgramPtr getProgram() const;

	// Serialization
	//void load( const std::string& name );
	//void save( const std::string& name );

protected:

	// Textures
	std::vector< TexturePtr > textures;

	// Shaders
	ProgramPtr program;
	//std::map< resources::ShaderPtr > shaders;

	std::string name;
};

//-----------------------------------//

typedef tr1::shared_ptr< Material > MaterialPtr;

//-----------------------------------//

} } // end namespaces