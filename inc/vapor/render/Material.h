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

#include "vapor/render/TextureManager.h"

namespace vapor {
	namespace render {

//-----------------------------------//

/**
 * Rendering material.
 */

class VAPOR_API Material
{
public:

	Material( const std::string& name );
	Material( const std::string& name, ProgramPtr program );
	~Material();

	// Gets the textual name of the material.
	const std::string& getName() const;

	// Adds a texture to the material.
	void setTexture( uint unit, const std::string& tex );

	// Gets the associated program.
	ProgramPtr getProgram() const;

	// Sets the associated program.
	void setProgram( ProgramPtr program );

	// Gets the textures in the material.
	const std::map< uint, TexturePtr >& getTextures() const;

	// Binds the material object.
	void bind();

	// Unbinds the material object.
	void unbind();

	// Serialization
	//void load( const std::string& name );
	//void save( const std::string& name );

protected:

	// Textures
	std::map< uint, TexturePtr > textures;
	typedef std::pair< const uint, TexturePtr > texPair;

	// Shaders
	ProgramPtr program;
	//std::map< resources::ShaderPtr > shaders;

	std::string name;

	TextureManager* tm;
};

//-----------------------------------//

typedef tr1::shared_ptr< Material > MaterialPtr;

//-----------------------------------//

} } // end namespaces