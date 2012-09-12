/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "Resources/Resource.h"
#include "Graphics/Resources/Image.h"
#include "Graphics/Texture.h"
#include "Graphics/Resources/ShaderMaterial.h"

NAMESPACE_GRAPHICS_BEGIN

//-----------------------------------//

enum struct BlendSource
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

enum struct BlendDestination
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

//-----------------------------------//

enum struct DepthCompare
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

//-----------------------------------//

API_GRAPHICS REFLECT_DECLARE_CLASS(Material)

class API_GRAPHICS Material : public Resource
{
	REFLECT_DECLARE_OBJECT(Material)

public:

	Material();
	Material( const String& name );
	Material( const Material& rhs );
	~Material();

	// Gets the associated resource group.
	GETTER(ResourceGroup, ResourceGroup, ResourceGroup::Materials)

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
	ACESSOR(DepthCompare, DepthCompare, depthCompare)

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
	GETTER(BlendSource, BlendSource, source)
	
	// Gets the blending options for this material.
	GETTER(BlendDestination, BlendDestination, destination)

	// Gets if blending is enabled.
	bool isBlendingEnabled() const;

	// Sets the blending options for this material.
	void setBlending( BlendSource, BlendDestination );

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
	DepthCompare depthCompare;
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
	BlendSource source;
	BlendDestination destination;
	bool _isBlendingEnabled;

	// Default line width.
	static float DefaultLineWidth;
};

TYPEDEF_RESOURCE_HANDLE_FROM_TYPE( Material );

API_GRAPHICS MaterialHandle MaterialCreate(Allocator*, const String& name);

//-----------------------------------//

NAMESPACE_GRAPHICS_END