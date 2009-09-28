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
	: uploaded( false ), img( img ), id( 0 )
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
	// TODO: check for maximum texture size the implementation
	// can handle...
	// TODO: check for OpenGL errors

	bind();

	// TODO: check the formats more thorougly
	glTexImage2D( GL_TEXTURE_2D, 0, 
		convertInternalFormat( img->getPixelFormat() ),
		img->getWidth(), img->getHeight(), 0,
		GL_RGBA, GL_UNSIGNED_BYTE, &img->getBuffer()[0] );

	glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	//gluBuild2DMipmaps( GL_TEXTURE_2D,
	//	convertInternalFormat( img->getPixelFormat() ),
	//	img->getWidth(), img->getHeight(),
	//	GL_RGBA, GL_UNSIGNED_BYTE, &img->getBuffer()[0] );

#ifdef VAPOR_DEBUG
	if( glGetError() != GL_NO_ERROR )
	{
		warn( "gl", "Could not upload texture object '%d'", id );
		uploaded = false;
		return false;
	}
#endif

	uploaded = true;
	return true;
}

//-----------------------------------//

void Texture::bind( int unit )
{

	glActiveTexture( GL_TEXTURE0+unit );
	glBindTexture( GL_TEXTURE_2D, id );
}

//-----------------------------------//

void Texture::unbind()
{
	glBindTexture( GL_TEXTURE_2D, 0 );

	//glDisable( GL_TEXTURE_2D );
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