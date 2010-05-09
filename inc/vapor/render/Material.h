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

FWD_DECL(render, Device)

namespace vapor { namespace render {

//-----------------------------------//

namespace BlendingSource
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

namespace BlendingDestination
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

typedef std::map< uint, TexturePtr > TextureMap;
typedef std::pair< const uint, TexturePtr > TextureMapPair;

/**
 * Rendering material.
 */

class VAPOR_API Material : public ReferenceCounted
{
	friend class render::Device;

public:

	//Material( const std::string& name );
	Material( const std::string& name, ProgramPtr program );
	Material( const std::string& name, const std::string& program = "diffuse" );

	// Gets the textual name of the material.
	IMPLEMENT_GETTER(Name, const std::string&, name);

	// Adds a texture to the material.
	void setTexture( uint unit, const std::string& tex );

	// Adds a texture to the material.
	void setTexture( uint unit, const TexturePtr& tex );

	// Adds an image as texture to the material.
	void setTexture( uint unit, const resources::ImagePtr& tex );

	// Sets the associated program.
	void setProgram( const std::string& name );

	// Gets the blending options for this material.
	IMPLEMENT_GETTER(BlendingSource, BlendingSource::Enum, src)
	
	// Gets the blending options for this material.
	IMPLEMENT_GETTER(BlendingDestination, BlendingDestination::Enum, dst)

	// Is blending enabled?
	// Blending is automatically enabled if you set a custom option.
	bool isBlendingEnabled() const;

	// Sets the blending options for this material.
	void setBlending( BlendingSource::Enum src, BlendingDestination::Enum dst );

	// Gets the textures in the material.
	IMPLEMENT_GETTER(Textures, const TextureMap&, textures)

	// Binds the material object.
	void bind();

	// Unbinds the material object.
	void unbind();

	// Serialization.
	//void serialize( Json::Value value );

	IMPLEMENT_ACESSOR(DepthWrite, bool, depthWrite)
	IMPLEMENT_ACESSOR(DepthTest, bool, depthTest)
	IMPLEMENT_ACESSOR(LineWidth, float, lineWidth)
	IMPLEMENT_ACESSOR(LineSmoothing, bool, lineSmooth)
	IMPLEMENT_ACESSOR(BackfaceCulling, bool, cullBackfaces)
	IMPLEMENT_ACESSOR(Program, ProgramPtr, program)

protected:

	// Common material initialization.
	void init();

	// Name of the material.
	std::string name;

	// Program of the material.
	ProgramPtr program;

	// Textures
	TextureMap textures;

	// Backface culling
	bool cullBackfaces;
	
	bool depthTest;
	bool depthWrite;

	// Line settings
	bool lineSmooth;
	float lineWidth;

	// Blending operations
	BlendingSource::Enum src;
	BlendingDestination::Enum dst;
	bool _isBlendingEnabled;

	// Default Line Width.
	static float DefaultLineWidth;
};

//-----------------------------------//

TYPEDEF_INTRUSIVE_POINTER_FROM_TYPE( Material );

//-----------------------------------//

} } // end namespaces