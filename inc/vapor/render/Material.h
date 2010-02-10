/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "vapor/render/Program.h"
#include "vapor/render/Texture.h"
#include "vapor/resources/Shader.h"
#include "vapor/render/TextureManager.h"
#include "vapor/render/GL.h"

namespace vapor { namespace render {

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

class VAPOR_API Material : public ReferenceCounted
{
public:

	//Material( const std::string& name );
	Material( const std::string& name, ProgramPtr program );
	Material( const std::string& name, const std::string& program = "diffuse" );
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

	// Sets the associated program.
	void setProgram( const std::string& name );

	// Gets the backface culling settings.
	bool getBackfaceCulling();

	// Sets the backface culling settings.
	void setBackfaceCulling( bool cullFaces );

	// Gets the line width of this material.
	float getLineWidth() const;

	// Sets the line width of this material.
	void setLineWidth( float width );

	// Gets the line smoothing of this material.
	bool getLineSmoothing() const;

	// Sets the line smoothing of this material.
	void setLineSmoothing( bool smooth );

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

	// Backface culling
	bool cullBackfaces;

	// Line settings
	bool lineSmooth;
	float lineWidth;

	float polygonOffset;
	
	// Textures
	std::map< uint, TexturePtr > textures;
	typedef std::pair< const uint, TexturePtr > texPair;

	// Program of the material.
	ProgramPtr program;
	
	// Name of the material.
	std::string name;
};

//-----------------------------------//

TYPEDEF_INTRUSIVE_POINTER_FROM_CLASS( Material );

//-----------------------------------//

} } // end namespaces