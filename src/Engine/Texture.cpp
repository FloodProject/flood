/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "vapor/PCH.h"
#include "vapor/render/Texture.h"
#include "vapor/render/Adapter.h"

using namespace vapor::resources;

namespace vapor { namespace render {

//-----------------------------------//

Texture::Texture( const Settings& settings )
	: width(settings.width), height(settings.height)
{
	init();
}

//-----------------------------------//

Texture::Texture( ushort width, ushort height )
	: width( width ), height( height )
{
	init();
}

//-----------------------------------//

Texture::Texture( const resources::ImagePtr image )
	: img( nullptr )
{
	assert( image != nullptr );
	
	init();
	setImage( image );
}

//-----------------------------------//

Texture::~Texture()
{
	glDeleteTextures( 1, &_id );
	
	if( glHasError("Could not delete texture object") )
		return;
}

//-----------------------------------//

void Texture::init()
{
	uploaded = false;
	_id = 0;

	generate();
}

//-----------------------------------//

bool Texture::generate()
{
	glGenTextures( 1, &_id );

	if( glHasError("Could not generate a new texture object") )
		return false;

	return true;
}

//-----------------------------------//

bool Texture::check()
{
	uint max_size = Adapter::getInstancePtr()->getMaxTextureSize();

	if( (width > max_size) || (height > max_size) )
	{
		warn( "gl", "Texture size is not supported (max: %dx%d)",
			max_size, max_size );		
		return false;
	}

	return true;
}

//-----------------------------------//

bool Texture::upload()
{
	bind();

	// TODO: check for OpenGL errors

	if( !check() ) return false;

	// TODO: check the formats more thoroughly
	glTexImage2D( GL_TEXTURE_2D, 0, 
		img ? convertInternalFormat( img->getPixelFormat() ) : GL_RGBA8,
		width, height, 0,
		img ? convertSourceFormat( img->getPixelFormat() ) : GL_RGBA,
		GL_UNSIGNED_BYTE, img ? &img->getBuffer()[0] : nullptr );

	configure();

	if( glHasError("Could not upload texture object") )
	{
		uploaded = false;
		return false;
	}

	unbind();

	uploaded = true;
	return true;
}

//-----------------------------------//

void Texture::configure()
{
	glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	// TODO: mipmaps and stuff
}

//-----------------------------------//

void Texture::setImage( const resources::ImagePtr& image )
{
	assert( image != nullptr );

	img = image;

	width = img->getWidth();
	height = img->getHeight();

	upload();
}

//-----------------------------------//

void Texture::bind( int unit )
{
	glActiveTexture( GL_TEXTURE0+unit );
	glBindTexture( GL_TEXTURE_2D, _id );
}

//-----------------------------------//

void Texture::unbind( int unit )
{
	glActiveTexture( GL_TEXTURE0+unit );
	glBindTexture( GL_TEXTURE_2D, 0 );
}

//-----------------------------------//

GLint Texture::convertInternalFormat( resources::PixelFormat::Enum fmt )
{
	switch( fmt )
	{
	case PixelFormat::R8G8B8A8:
		return GL_RGBA8;
	case PixelFormat::R8G8B8:
		return GL_RGB8;
	default:
		warn( "GL", "Implement support for more pixel formats" );
		return GL_RGB8;
	}
}

//-----------------------------------//

GLint Texture::convertSourceFormat( resources::PixelFormat::Enum fmt )
{
	switch( fmt )
	{
	case PixelFormat::R8G8B8A8:
		return GL_RGBA;
	case PixelFormat::R8G8B8:
		return GL_RGB;
	default:
		warn( "GL", "Implement support for more pixel formats" );
		return GL_RGB;
	}
}

//-----------------------------------//

uint Texture::id() const
{
	return _id;
}

//-----------------------------------//

} } // end namespaces