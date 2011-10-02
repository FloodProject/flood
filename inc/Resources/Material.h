/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "Resources/Resource.h"
#include "Resources/Image.h"

#define GL_ZERO                0
#define GL_ONE                 1
#define GL_SRC_COLOR           0x0300
#define GL_ONE_MINUS_SRC_COLOR 0x0301
#define GL_SRC_ALPHA           0x0302
#define GL_ONE_MINUS_SRC_ALPHA 0x0303
#define GL_DST_ALPHA           0x0304
#define GL_ONE_MINUS_DST_ALPHA 0x0305
#define GL_DST_COLOR           0x0306
#define GL_ONE_MINUS_DST_COLOR 0x0307
#define GL_SRC_ALPHA_SATURATE  0x0308

#define GL_NEVER    0x0200
#define GL_LESS     0x0201
#define GL_EQUAL    0x0202
#define GL_LEQUAL   0x0203
#define GL_GREATER  0x0204
#define GL_NOTEQUAL 0x0205
#define GL_GEQUAL   0x0206
#define GL_ALWAYS   0x0207

NAMESPACE_RESOURCES_BEGIN

//-----------------------------------//

struct BlendSource
{
	enum Enum
	{
		Zero                    = GL_ZERO,
		One                     = GL_ONE,
		DestinationColor        = GL_DST_COLOR,
		InverseDestinationColor = GL_ONE_MINUS_DST_COLOR,
		SourceAlpha             = GL_SRC_ALPHA,
		InverseSourceAlpha      = GL_ONE_MINUS_SRC_ALPHA,
		DestinationAlpha        = GL_DST_ALPHA,
		InverseDestinationAlpha = GL_ONE_MINUS_DST_ALPHA,
		SourceAlphaSaturate     = GL_SRC_ALPHA_SATURATE
	};
};

struct BlendDestination
{
	enum Enum
	{
		Zero                    = GL_ZERO,
		One                     = GL_ONE,
		SourceColor             = GL_SRC_COLOR,
		InverseSourceColor      = GL_ONE_MINUS_SRC_COLOR,
		SourceAlpha             = GL_SRC_ALPHA,
		InverseSourceAlpha      = GL_ONE_MINUS_SRC_ALPHA,
		DestinationAlpha        = GL_DST_ALPHA,
		InverseDestinationAlpha = GL_ONE_MINUS_DST_ALPHA
	};
};

//-----------------------------------//

struct DepthCompare
{
	enum Enum
	{
		Never          = GL_NEVER,
		Less           = GL_LESS,
		Equal          = GL_EQUAL,
		LessOrEqual    = GL_LEQUAL,
		Greater        = GL_GREATER,
		NotEqual       = GL_NOTEQUAL,
		GreaterOrEqual = GL_GEQUAL,
		Always         = GL_ALWAYS,
	};
};

//-----------------------------------//

struct TextureFilterMode
{
	enum Enum
	{
		Nearest,
		Linear,
	};
};

struct TextureMipMode
{
	enum Enum
	{
		Nearest,
		Linear,
	};
};

struct TextureWrapMode
{
	enum Enum
	{
		Repeat,
		Clamp,
		ClampToBorder
	};
};

struct TextureUnit
{
	TextureUnit();

	// Texture unit id.
	uint8 unit;

	// Image used on this texture unit.
	ImageHandle image;

	// Keeps track if texture modes are overriden.
	bool overrideModes;

	// Gets the texture filtering mode.
	GETTER(FilterMode, TextureFilterMode::Enum, filter)

	// Gets the texture mip filtering mode.
	GETTER(MipMode, TextureMipMode::Enum, mip)

	// Gets the texture wrap mode.
	GETTER(WrapMode, TextureWrapMode::Enum, wrap)

	// Sets the texture filtering mode.
	void setFilterMode(TextureFilterMode::Enum);

	// Sets the texture mip filtering mode.
	void setMipMode(TextureMipMode::Enum);

	// Sets the texture wrap mode.
	void setWrapMode(TextureWrapMode::Enum);

protected:

	TextureFilterMode::Enum filter;
	TextureMipMode::Enum mip;
	TextureWrapMode::Enum wrap;
};

typedef std::map<uint8, TextureUnit> TextureUnitMap;

//-----------------------------------//

REFLECT_DECLARE_CLASS(Material)

class API_RESOURCE Material : public Resource
{
	REFLECT_DECLARE_OBJECT(Material)

public:

	Material();
	Material( const String& name, const String& program = "VertexColor" );
	Material( const Material& rhs );
	~Material();

	// Gets the associated resource group.
	GETTER(ResourceGroup, ResourceGroup::Enum, ResourceGroup::Materials)

	// Gets the textual name of the material.
	ACESSOR(Name, const String&, name);

	// Gets/sets the associated program.
	ACESSOR(Program, const String&, program)
	
	// Gets/sets the depth writing of the material.
	ACESSOR(DepthWrite, bool, depthWrite)

	// Gets/sets the depth testing of the material.
	ACESSOR(DepthTest, bool, depthTest)

	// Gets/sets the depth testing of the material.
	ACESSOR(DepthCompare, DepthCompare::Enum, depthCompare)

	// Gets/sets the depth range of the material.
	ACESSOR(DepthRange, Vector2, depthRange)

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

	// Gets if blending is enabled.
	bool isBlendingEnabled() const;

	// Sets the blending options for this material.
	void setBlending( BlendSource::Enum, BlendDestination::Enum );

	// Sets a texture to the material.
	void setTexture( uint8 unit, const String& name );

	// Sets an image as texture to the material.
	void setTexture( uint8 unit, const ImageHandle& image );

	// Gets a texture of the material.
	ImageHandle getTexture( uint8 unit );

	// Gets a texture unit of the material.
	TextureUnit& getTextureUnit( uint8 unit );

	// Texture units.
	TextureUnitMap textureUnits;

	// Initialization.
	void init();

public:

	// Name of the material.
	String name;

	// Program of the material.
	String program;

	// Backface culling.
	bool cullBackfaces;
	
	// Depth settings.
	DepthCompare::Enum depthCompare;
	bool depthTest;
	bool depthWrite;
	Vector2 depthRange;

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

TYPEDEF_RESOURCE_HANDLE_FROM_TYPE( Material );

MaterialHandle MaterialCreate(Allocator*, const String& name);

//-----------------------------------//

NAMESPACE_RESOURCES_END