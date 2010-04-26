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

Texture::Texture( const resources::ImagePtr& _image )
	: image( nullptr )
{
	assert( _image != nullptr );
	
	init();
	setImage( _image );
	configure();
}

//-----------------------------------//

Texture::Texture( const Settings& settings, PixelFormat::Enum format )
	: width(settings.width), height(settings.height), format(format)
{
	init();
	upload();
	configure();
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
	uint max = Adapter::getInstancePtr()->getMaxTextureSize();

	if( (width > max) || (height > max) )
	{
		warn( "gl", "Texture size is not supported (max: %dx%d)", max, max );		
		return false;
	}

	return true;
}

//-----------------------------------//

bool Texture::upload()
{
	if( !check() )
		return false;

	bind();

	glTexImage2D( GL_TEXTURE_2D, 0, convertInternalFormat(format),
		width, height, 0, convertSourceFormat(format),
		GL_UNSIGNED_BYTE, image ? &image->getBuffer()[0] : nullptr );

	if( glHasError("Could not upload pixel data to texture object") )
	{
		uploaded = false;
		return false;
	}

	uploaded = true;
	return true;
}

//-----------------------------------//

void Texture::configure()
{
	glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T, GL_CLAMP);

	glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	// TODO: mipmaps and stuff
}

//-----------------------------------//

void Texture::setImage( const resources::ImagePtr& _image )
{
	assert( _image != nullptr );

	image = _image;
	width = image->getWidth();
	height = image->getHeight();
	format = image->getPixelFormat();

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

GLint Texture::convertInternalFormat( PixelFormat::Enum format )
{
	switch( format )
	{
	case PixelFormat::R8G8B8A8:
		return GL_RGBA8;
	case PixelFormat::R8G8B8:
		return GL_RGB8;
	case PixelFormat::Depth:
		return GL_DEPTH_COMPONENT;
	}

	assert( 0 && "Implement support for more pixel formats" );
	return 0;
}

//-----------------------------------//

GLint Texture::convertSourceFormat( PixelFormat::Enum format )
{
	switch( format )
	{
	case PixelFormat::R8G8B8A8:
		return GL_RGBA;
	case PixelFormat::R8G8B8:
		return GL_RGB;
	case PixelFormat::Depth:
		return GL_DEPTH_COMPONENT;
	}

	assert( 0 && "Implement support for more pixel formats" );
	return 0;
}

//-----------------------------------//

uint Texture::id() const
{
	return _id;
}

//-----------------------------------//

} } // end namespaces