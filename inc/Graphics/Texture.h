/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

#include "Resources/Resource.h"
#include "Graphics/Resources/Image.h"
#include "Graphics/RenderTarget.h"

NAMESPACE_GRAPHICS_BEGIN

//-----------------------------------//

enum struct TextureFilterMode
{
	Nearest,
	Linear,
};

enum struct TextureMipMode
{
	Nearest,
	Linear,
};

enum struct TextureWrapMode
{
	Repeat,
	Clamp,
	ClampToBorder,
	ClampToEdge
};

//-----------------------------------//

/**
 * Represents a texture on the rendering API. This will be basically an
 * the same content of a resource image stored on the graphics card, but 
 * it is not guaranteed that the GPU internal format will be the same.
 * You can also create textures that are not backed by an image.
 */

enum struct TextureTarget
{
	Target2D,
	Target3D
};

class API_GRAPHICS Texture : public ReferenceCounted
{
public:
	
    Texture();
	~Texture();

	// Allocates texture space for the given size and format.
	void allocate(const Vector2i& size, PixelFormat);

	// Gets the associated identifier.
	GETTER(Id, uint32, id)

	// Gets the associated pixel format.
	GETTER(PixelFormat, PixelFormat, format)

	// Gets the associated image.
	GETTER(Image, const ImageHandle&, image)

	// Sets the associated image.
	void setImage( const ImageHandle& image );

	// Reads the texture as an image.
	Image* readImage() const;

	// Gets the expected size of the image.
	uint32 getExpectedSize() const;

    bool isUploaded() const;

    void setUploaded();

	uint32 id;
	TextureTarget target;
	
	uint16 width;
	uint16 height;
	PixelFormat format;
	float anisotropicFilter;
	
	ImageHandle image;

private:

    bool uploaded;
    uint32 uploadedImageTimestamp;

};

//-----------------------------------//

TYPEDEF_INTRUSIVE_POINTER_FROM_TYPE( Texture );

struct API_GRAPHICS TextureUnit
{
	TextureUnit();

	// Texture unit id.
	uint8 unit;

	// Image used on this texture unit.
	ImageHandle image;

	// Keeps track if texture modes are overriden.
	bool overrideModes;

	// Gets the texture filtering mode.
	GETTER(FilterMode, TextureFilterMode, filter)

	// Gets the texture mip filtering mode.
	GETTER(MipMode, TextureMipMode, mip)

	// Gets the texture wrap mode.
	GETTER(WrapMode, TextureWrapMode, wrap)

	// Sets the texture filtering mode.
	void setFilterMode(TextureFilterMode);

	// Sets the texture mip filtering mode.
	void setMipMode(TextureMipMode);

	// Sets the texture wrap mode.
	void setWrapMode(TextureWrapMode);

protected:

	TextureFilterMode filter;
	TextureMipMode mip;
	TextureWrapMode wrap;
};

typedef std::map<uint8, TextureUnit> TextureUnitMap;

//-----------------------------------//

NAMESPACE_GRAPHICS_END