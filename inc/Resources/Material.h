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
#include "Resources/ShaderMaterial.h"

NAMESPACE_RESOURCES_BEGIN

//-----------------------------------//

struct BlendSource
{
	enum Enum
	{
		Zero,
		One,
		DestinationColor,
		InverseDestinationColor,
		SourceAlpha,
		InverseSourceAlpha,
		DestinationAlpha,
		InverseDestinationAlpha,
		SourceAlphaSaturate
	};
};

struct BlendDestination
{
	enum Enum
	{
		Zero,
		One,
		SourceColor,
		InverseSourceColor,
		SourceAlpha,
		InverseSourceAlpha,
		DestinationAlpha,
		InverseDestinationAlpha
	};
};

//-----------------------------------//

struct DepthCompare
{
	enum Enum
	{
		Never,
		Less,
		Equal,
		LessOrEqual,
		Greater,
		NotEqual,
		GreaterOrEqual,
		Always
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
		ClampToBorder,
		ClampToEdge
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
	Material( const String& name );
	Material( const Material& rhs );
	~Material();

	// Gets the associated resource group.
	GETTER(ResourceGroup, ResourceGroup::Enum, ResourceGroup::Materials)

	// Gets the textual name of the material.
	ACESSOR(Name, const String&, name);

	// Gets/sets the shader of the material.
	ACESSOR(Shader, const ShaderMaterialHandle&, shader)

	// Sets the shader of the material.
	void setShader(const String& name);

	// Gets/sets the depth writing of the material.
	ACESSOR(DepthWrite, bool, depthWrite)

	// Gets/sets the depth testing of the material.
	ACESSOR(DepthTest, bool, depthTest)

	// Gets/sets the depth testing of the material.
	ACESSOR(DepthCompare, DepthCompare::Enum, depthCompare)

	// Gets/sets the depth range of the material.
	ACESSOR(DepthRange, Vector2, depthRange)

	// Gets/sets the depth offset of the material.
	ACESSOR(DepthOffset, Vector2, depthOffset)

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
	ShaderMaterialHandle shader;

	// Backface culling.
	bool cullBackfaces;
	
	// Depth settings.
	DepthCompare::Enum depthCompare;
	bool depthTest;
	bool depthWrite;
	Vector2 depthRange;
	Vector2 depthOffset;

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