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