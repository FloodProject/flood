/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "Graphics/RenderTarget.h"
#include "Resources/Image.h"
#include "Resources/Material.h"

NAMESPACE_GRAPHICS_BEGIN

//-----------------------------------//

/**
 * Represents a texture on the rendering API. This will be basically an
 * the same content of a resource image stored on the graphics card, but 
 * it is not guaranteed that the GPU internal format will be the same.
 * You can also create textures that are not backed by an image.
 */

class API_GRAPHICS Texture : public ReferenceCounted
{
public:

	enum TextureTarget
	{
		Target2D,
		Target3D
	};
	
	Texture();
	~Texture();

	// Allocates texture space for the given size and format.
	void allocate(const Vector2i& size, PixelFormat::Enum);

	// Gets the associated identifier.
	GETTER(Id, uint32, id)

	// Gets the associated pixel format.
	GETTER(PixelFormat, PixelFormat::Enum, format)

	// Gets the associated image.
	GETTER(Image, const Image*, image)

	// Sets the associated image.
	void setImage( Image* image );

	// Reads the texture as an image.
	Image* readImage() const;

	// Gets the expected size of the image.
	uint32 getExpectedSize() const;

	uint32 id;
	TextureTarget target;
	bool uploaded;
	
	uint16 width;
	uint16 height;
	PixelFormat::Enum format;
	float anisotropicFilter;
	
	Image* image;
};

TYPEDEF_INTRUSIVE_POINTER_FROM_TYPE( Texture );

//-----------------------------------//

NAMESPACE_GRAPHICS_END