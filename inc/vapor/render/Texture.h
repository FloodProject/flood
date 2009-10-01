/************************************************************************
*
* vaporEngine (2008-2009)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#pragma once

#include "vapor/Platform.h"

#include "vapor/resources/Image.h"
#include "vapor/render/GL.h"

namespace vapor {
	namespace render {

//-----------------------------------//

/**
 * Represents a texture on the rendering API. This will be basically an
 * the same content of a resource image stored on the graphics card.
 * TODO: compressed texture?
 */

class VAPOR_API Texture
{
public:

	Texture( resources::ImagePtr );
	~Texture();

	// Uploads the image data to the graphics card.
	bool upload();

	// Binds the texture object.
	void bind( int unit = 0 );

	// Unbinds the texture object.
	void unbind( int unit = 0 );

private:

	GLint convertInternalFormat( resources::PixelFormat::Enum fmt );

	// OpenGL texture object id
	GLuint id;

	resources::ImagePtr img;

	bool uploaded;
};

//-----------------------------------//

typedef tr1::shared_ptr< Texture > TexturePtr;

//-----------------------------------//

} } // end namespaces