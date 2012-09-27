/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

#include "Resources/Resource.h"
#include "Graphics/Resources/Image.h"
#include "Graphics/Texture.h"
#include "Graphics/Resources/ShaderMaterial.h"

NAMESPACE_GRAPHICS_BEGIN

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

	// Initializes the material.
	void init();

	// Gets the associated resource group.
	GETTER(ResourceGroup, ResourceGroup, ResourceGroup::Materials)

	// Gets the textual name of the material.
	ACCESSOR(Name, const String&, name);

	// Gets/sets the shader of the material.
	ACCESSOR(Shader, const ShaderMaterialHandle&, shader)

	// Sets the shader of the material.
	void setShader(const String& name);

	// Gets/sets the render mode.
	ACCESSOR(PrimitiveRasterMode, PrimitiveRasterMode, mode)

	// Gets/sets the depth writing of the material.
	ACCESSOR(DepthWrite, bool, depthWrite)

	// Gets/sets the depth testing of the material.
	ACCESSOR(DepthTest, bool, depthTest)

	// Gets/sets the depth testing of the material.
	ACCESSOR(DepthCompare, DepthCompare, depthCompare)

	// Gets/sets the depth range of the material.
	ACCESSOR(DepthRange, Vector2, depthRange)

	// Gets/sets the depth offset of the material.
	ACCESSOR(DepthOffset, Vector2, depthOffset)

	// Gets/sets the alpha testing of the material.
	ACCESSOR(AlphaTest, bool, alphaTest)

	// Gets/sets the line width of the material.
	ACCESSOR(LineWidth, float, lineWidth)

	// Gets/sets the line smoothing of the material.
	ACCESSOR(LineSmoothing, bool, lineSmooth)

	// Gets/sets the backface culling of the material.
	ACCESSOR(BackfaceCulling, bool, cullBackfaces)

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

public:

	// Name of the material.
	String name;

	// Program of the material.
	ShaderMaterialHandle shader;
};

TYPEDEF_RESOURCE_HANDLE_FROM_TYPE( Material );

API_GRAPHICS MaterialHandle MaterialCreate(Allocator*, const String& name);

//-----------------------------------//

NAMESPACE_GRAPHICS_END