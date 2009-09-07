/************************************************************************
*
* vaporEngine (2008-2009)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#include "vapor/PCH.h"

#include "vapor/render/Texture.h"

using namespace vapor::resources;

namespace vapor {
	namespace render {

//-----------------------------------//

Texture::Texture( resources::ImagePtr img )
	: uploaded( false ), img( img )
{
	glGenTextures( 1, &id );

	upload();

	unbind();
}

//-----------------------------------//

Texture::~Texture()
{
	glDeleteTextures( 1, &id );
}

//-----------------------------------//

bool Texture::upload()
{
	// TODO: check for OpenGL errors

	bind();

	// TODO: check the formats more thorougly
	glTexImage2D( GL_TEXTURE_2D, 0, 
		convertInternalFormat( img->getPixelFormat() ),
		img->getWidth(), img->getHeight(), 0,
		GL_RGBA, GL_UNSIGNED_BYTE, &img->getBuffer()[0] );

	uploaded = true;

	return true;
}

//-----------------------------------//

void Texture::bind()
{
	glBindTexture( GL_TEXTURE_2D, id );
}

//-----------------------------------//

void Texture::unbind()
{
	glBindTexture( GL_TEXTURE_2D, 0 );
}

//-----------------------------------//

GLint Texture::convertInternalFormat( resources::PixelFormat::Enum fmt )
{
	switch( fmt )
	{
	case PixelFormat::R8G8B8A8:
		return GL_RGBA8;
	default:
		return GL_RGBA8;
	}
}

//-----------------------------------//

} } // end namespaces