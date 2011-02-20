/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "render/Program.h"
#include "render/Texture.h"
#include "Resources/Shader.h"

#define GL_ZERO 0
#define GL_ONE 1
#define GL_SRC_COLOR 0x0300
#define GL_ONE_MINUS_SRC_COLOR 0x0301
#define GL_SRC_ALPHA 0x0302
#define GL_ONE_MINUS_SRC_ALPHA 0x0303
#define GL_DST_ALPHA 0x0304
#define GL_ONE_MINUS_DST_ALPHA 0x0305
#define GL_DST_COLOR 0x0306
#define GL_ONE_MINUS_DST_COLOR 0x0307
#define GL_SRC_ALPHA_SATURATE 0x0308

#define GL_NEVER 0x0200
#define GL_LESS 0x0201
#define GL_EQUAL 0x0202
#define GL_LEQUAL 0x0203
#define GL_GREATER 0x0204
#define GL_NOTEQUAL 0x0205
#define GL_GEQUAL 0x0206
#define GL_ALWAYS 0x0207

namespace vapor {

//-----------------------------------//

struct BlendSource
{
	enum Enum
	{
		Zero = GL_ZERO,
		One = GL_ONE,
		DestinationColor = GL_DST_COLOR,
		InverseDestinationColor = GL_ONE_MINUS_DST_COLOR,
		SourceAlpha = GL_SRC_ALPHA,
		InverseSourceAlpha = GL_ONE_MINUS_SRC_ALPHA,
		DestinationAlpha = GL_DST_ALPHA,
		InverseDestinationAlpha = GL_ONE_MINUS_DST_ALPHA,
		SourceAlphaSaturate = GL_SRC_ALPHA_SATURATE
	};
};

//-----------------------------------//

struct BlendDestination
{
	enum Enum
	{
		Zero = GL_ZERO,
		One = GL_ONE,
		SourceColor = GL_SRC_COLOR,
		InverseSourceColor = GL_ONE_MINUS_SRC_COLOR,
		SourceAlpha = GL_SRC_ALPHA,
		InverseSourceAlpha = GL_ONE_MINUS_SRC_ALPHA,
		DestinationAlpha = GL_DST_ALPHA,
		InverseDestinationAlpha = GL_ONE_MINUS_DST_ALPHA
	};
};

//-----------------------------------//

struct DepthCompare
{
	enum Enum
	{
		Never = GL_NEVER,
		Less = GL_LESS,
		Equal = GL_EQUAL,
		LessOrEqual = GL_LEQUAL,
		Greater = GL_GREATER,
		NotEqual = GL_NOTEQUAL,
		GreaterOrEqual = GL_GEQUAL,
		Always = GL_ALWAYS,
	};
};

//-----------------------------------//

typedef std::map< uint, TexturePtr > TextureMap;
typedef std::pair< const uint, TexturePtr > TextureMapPair;

/**
 * Rendering material.
 */

class VAPOR_API Material : public ReferenceCounted
{
	friend class RenderDevice;

public:

	//Material( const std::string& name );
	Material( const std::string& name, const std::string& program = "VertexColor" );
	Material( const Material& rhs );
	~Material();

	// Gets the textual name of the material.
	GETTER(Name, const std::string&, name);

	// Sets a texture to the material.
	void setTexture( uint unit, const std::string& tex );

	// Sets a texture to the material.
	void setTexture( uint unit, const TexturePtr& tex );

	// Sets an image as texture to the material.
	void setTexture( uint unit, const ImagePtr& tex );

	// Gets a texture of the material.
	TexturePtr getTexture( uint unit );

	// Gets/sets the associated program.
	ACESSOR(Program, const std::string&, program)
	
	// Gets the associated program.	
	ProgramPtr getProgram();

	// Gets/sets the depth writing of the material.
	ACESSOR(DepthWrite, bool, depthWrite)

	// Gets/sets the depth testing of the material.
	ACESSOR(DepthTest, bool, depthTest)

	// Gets/sets the depth testing of the material.
	ACESSOR(DepthCompare, DepthCompare::Enum, depthCompare)

	// Gets/sets the alpha testing of the material.
	ACESSOR(AlphaTest, bool, alphaTest)

	// Gets/sets the line width of the material.
	ACESSOR(LineWidth, float, lineWidth)

	// Gets/sets the line smoothing of the material.
	ACESSOR(LineSmoothing, bool, lineSmooth)

	// Gets/sets the backface culling of the material.
	ACESSOR(BackfaceCulling, bool, cullBackfaces)

	// Gets the blending options for this material.
	GETTER(BlendSource, BlendSource::Enum, source)
	
	// Gets the blending options for this material.
	GETTER(BlendDestination, BlendDestination::Enum, destination)

	// Is blending enabled?
	// Blending is automatically enabled if you set a custom option.
	bool isBlendingEnabled() const;

	// Sets the blending options for this material.
	void setBlending( BlendSource::Enum, BlendDestination::Enum );

	// Gets the textures in the material.
	GETTER(Textures, const TextureMap&, textures)

	// Binds the material object.
	void bindTextures(bool bindUniforms);

	// Unbinds the material object.
	void unbindTextures();

protected:

	// Initialization.
	void init();

	// Name of the material.
	std::string name;

	// Program of the material.
	std::string program;

	// Textures.
	TextureMap textures;

	// Backface culling.
	bool cullBackfaces;
	
	// Depth settings.
	DepthCompare::Enum depthCompare;
	bool depthTest;
	bool depthWrite;

	// Alpha settings.
	bool alphaTest;

	// Line settings.
	bool lineSmooth;
	float lineWidth;

	// Blending settings.
	BlendSource::Enum source;
	BlendDestination::Enum destination;
	bool _isBlendingEnabled;

	// Default line width.
	static float DefaultLineWidth;
};

TYPEDEF_INTRUSIVE_POINTER_FROM_TYPE( Material );

//-----------------------------------//

} // end namespace