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
#include "vapor/render/GL.h"

namespace vapor { namespace render {

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
	
	Texture( const resources::ImagePtr& );
	Texture( const Settings&, resources::PixelFormat::Enum = resources::PixelFormat::R8G8B8A8 );
	~Texture();

	// Generates a new texture id.
	bool generate();

	// Uploads the image data to the graphics card.
	bool upload();

	// Binds the texture object.
	void bind( int unit = 0 );

	// Unbinds the texture object.
	void unbind( int unit = 0 );

	// Configures the texture settings.
	void configure();

	// Checks if the texture size is supported.
	bool check();

	// Gets the associated identifier.
	uint id() const;

	// Gets/sets the associated image.
	IMPLEMENT_GETTER(Image, resources::ImagePtr, image)
	void setImage( const resources::ImagePtr& );

	// Gets the associated pixel format.
	IMPLEMENT_GETTER(PixelFormat, resources::PixelFormat::Enum, format)


protected:

	void init();

	GLint convertSourceFormat( resources::PixelFormat::Enum );
	GLint convertInternalFormat( resources::PixelFormat::Enum );

	// OpenGL texture object id
	GLuint _id;

	uint width, height;
	resources::ImagePtr image;
	resources::PixelFormat::Enum format;

	bool uploaded;
};

//-----------------------------------//

TYPEDEF_INTRUSIVE_POINTER_FROM_TYPE( Texture );

//-----------------------------------//

} } // end namespaces