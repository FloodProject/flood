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

#include "vapor/render/GL.h"

#include "vapor/render/TextureManager.h"

namespace vapor {
	namespace render {

//-----------------------------------//

namespace BlendingOperationSource
{
	enum Enum 
	{ 
		Zero = GL_ZERO,
		One = GL_ONE,
		DestinationColor = GL_DST_COLOR,
		OneMinusDestinationColor = GL_ONE_MINUS_DST_COLOR,
		SourceAlpha = GL_SRC_ALPHA,
		OneMinusSourceAlpha = GL_ONE_MINUS_SRC_ALPHA,
		DestinationAlpha = GL_DST_ALPHA,
		OneMinusDestinationAlpha = GL_ONE_MINUS_DST_ALPHA,
		SourceAlphaSaturate = GL_SRC_ALPHA_SATURATE
	};
}

//-----------------------------------//

namespace BlendingOperationDestination
{
	enum Enum
	{
		Zero = GL_ZERO,
		One = GL_ONE,
		SourceColor = GL_SRC_COLOR,
		OneMinusSourceColor = GL_ONE_MINUS_SRC_COLOR,
		SourceAlpha = GL_SRC_ALPHA,
		OneMinusSourceAlpha = GL_ONE_MINUS_SRC_ALPHA,
		DestinationAlpha = GL_DST_ALPHA,
		OneMinusDestinationAlpha = GL_ONE_MINUS_DST_ALPHA,
	};
}

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

	// Adds a texture to the material.
	void setTexture( uint unit, TexturePtr tex );

	// Gets the associated program.
	ProgramPtr getProgram() const;

	// Sets the associated program.
	void setProgram( ProgramPtr program );

	// Gets the blending options for this material.
	BlendingOperationSource::Enum getSourceBlendingOperation();
	
	// Gets the blending options for this material.
	BlendingOperationDestination::Enum getDestinationBlendingOperation();

	// Is blending enabled?
	// Blending is automatically enabled if you set a custom option.
	bool isBlendingEnabled();

	// Sets the blending options for this material.
	void setBlending( BlendingOperationSource::Enum src, 
		BlendingOperationDestination::Enum dst );

	// Gets the textures in the material.
	const std::map< uint, TexturePtr >& getTextures() const;

	// Binds the material object.
	void bind();

	// Unbinds the material object.
	void unbind();

	// Serialization.
	//void load( const std::string& name );
	//void save( const std::string& name );

protected:

	// Blending operations
	BlendingOperationSource::Enum src;
	BlendingOperationDestination::Enum dst;
	bool _isBlendingEnabled;

	// Textures
	std::map< uint, TexturePtr > textures;
	typedef std::pair< const uint, TexturePtr > texPair;

	// Shaders
	ProgramPtr program;
	std::string name;
};

//-----------------------------------//

TYPEDEF_SHARED_POINTER_FROM_CLASS( Material );

//-----------------------------------//

} } // end namespaces