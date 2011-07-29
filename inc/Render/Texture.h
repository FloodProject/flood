/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "Render/Target.h"
#include "Resources/Image.h"

NAMESPACE_ENGINE_BEGIN

//-----------------------------------//

/**
 * Texture filtering modes.
 */

namespace TextureFiltering
{
	enum Enum
	{
		Nearest,
		Linear
	};
}

//-----------------------------------//

/**
 * Represents a texture on the rendering API. This will be basically an
 * the same content of a resource image stored on the graphics card, but 
 * it is not guaranteed that the GPU internal format will be the same.
 * You can also create textures that are not backed by an image.
 */

class API_ENGINE Texture : public ReferenceCounted
{
public:
	
	Texture( const ImagePtr& );
	Texture( const Settings&, PixelFormat::Enum = PixelFormat::R8G8B8A8 );
	
	~Texture();

	// Gets the associated identifier.
	GETTER(Id, uint32, id)

	// Gets the associated pixel format.
	GETTER(PixelFormat, PixelFormat::Enum, format)

	// Gets the associated image.
	GETTER(Image, const Image*, image)

	// Sets the associated image.
	void setImage( Image* image );

	// Reads the texture as an image.
	ImagePtr readImage() const;

	// Binds the texture object.
	void bind( int unit = 0 ) const;

	// Unbinds the texture object.
	void unbind( int unit = 0 ) const;

protected:

	// Initializes the texture.
	void init();

	// Generates a new texture id.
	bool generate();

	// Checks if the texture size is supported.
	bool check();

	// Configures the texture settings.
	void configure();

	// Uploads the image data to the graphics card.
	bool upload();

	// Gets the expected size of the image.
	uint getExpectedSize() const;

	// Internal conversion methods.
	int convertSourceFormat( PixelFormat::Enum ) const;
	int convertInternalFormat( PixelFormat::Enum ) const;
	int convertFilterFormat( TextureFiltering::Enum ) const;

	uint32 id;
	uint32 target;
	bool uploaded;
	
	uint32 width;
	uint32 height;
	PixelFormat::Enum format;

	TextureFiltering::Enum minFilter;
	TextureFiltering::Enum maxFilter;
	float anisotropicFilter;
	
	Image* image;
};

TYPEDEF_INTRUSIVE_POINTER_FROM_TYPE( Texture );

//-----------------------------------//

NAMESPACE_ENGINE_END