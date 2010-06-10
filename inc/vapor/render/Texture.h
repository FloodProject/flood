/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "vapor/render/Target.h"
#include "vapor/resources/Image.h"

namespace vapor {

//-----------------------------------//

/**
 * Represents a texture on the rendering API. This will be basically an
 * the same content of a resource image stored on the graphics card, but 
 * it is not guaranteed that the GPU internal format will be the same.
 * You can also create textures that are not backed by an image.
 */

class VAPOR_API Texture : public ReferenceCounted
{
public:
	
	Texture( const ImagePtr& );
	Texture( const Settings&, PixelFormat::Enum = PixelFormat::R8G8B8A8 );
	~Texture();

	// Generates a new texture id.
	bool generate();

	// Uploads the image data to the graphics card.
	bool upload();

	// Binds the texture object.
	void bind( int unit = 0 ) const;

	// Unbinds the texture object.
	void unbind( int unit = 0 ) const;

	// Configures the texture settings.
	void configure();

	// Checks if the texture size is supported.
	bool check();

	// Gets the associated identifier.
	uint id() const;

	// Reads the texture as an image.
	ImagePtr readImage() const;

	// Gets the expected size of the image.
	uint getExpectedSize() const;

	// Gets/sets the associated image.
	IMPLEMENT_GETTER(Image, ImagePtr, image)
	void setImage( const ImagePtr& );

	// Gets the associated pixel format.
	IMPLEMENT_GETTER(PixelFormat, PixelFormat::Enum, format)

protected:

	void init();

	int convertSourceFormat( PixelFormat::Enum ) const;
	int convertInternalFormat( PixelFormat::Enum ) const;
	int convertGetFormat( PixelFormat::Enum ) const;

	// OpenGL texture object id
	uint _id;
	uint target;

	uint width, height;
	ImagePtr image;
	PixelFormat::Enum format;

	bool uploaded;
};

TYPEDEF_INTRUSIVE_POINTER_FROM_TYPE( Texture );

//-----------------------------------//

} // end namespace