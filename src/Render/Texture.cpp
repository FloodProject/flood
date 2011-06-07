/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Engine/API.h"
#include "Render/Texture.h"
#include "Render/Adapter.h"
#include "Render/Device.h"
#include "Render/GL.h"

namespace vapor {

//-----------------------------------//

Texture::Texture( const ImagePtr& newImage )
{
	init();
	setImage( newImage.get() );
	configure();
}

//-----------------------------------//

Texture::Texture( const Settings& settings, PixelFormat::Enum format )
	: width(settings.width)
	, height(settings.height)
	, format(format)
{
	init();
	upload();
	configure();
}

//-----------------------------------//

Texture::~Texture()
{
	glDeleteTextures( 1, (GLuint*) &id );
	
	if( glHasError("Could not delete texture object") )
		return;

	LogDebug("Deleting texture from OpenGL");
}

//-----------------------------------//

void Texture::init()
{
	id = 0;
	target = GL_TEXTURE_2D;
	uploaded = false;
	image = nullptr;

	minFilter = TextureFiltering::Linear;
	maxFilter = TextureFiltering::Linear;
	anisotropicFilter = 1.0f;

	generate();
}

//-----------------------------------//

bool Texture::generate()
{
	glGenTextures( 1, (GLuint*) &id );

	if( glHasError("Could not generate a new texture object") )
		return false;

	return true;
}

//-----------------------------------//

bool Texture::check()
{
	uint max;
	glGetIntegerv( GL_MAX_TEXTURE_SIZE, (GLint*) &max );
	
	if(width > max || height > max)
	{
		LogWarn( "Texture size is not supported (max: %dx%d)", max, max );		
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

	bool haveImage = image && !image->getBuffer().empty();

	glTexImage2D( target, 0, 
		convertInternalFormat(format),
		width, height, 0,
		convertSourceFormat(format),
		/*(format == PixelFormat::Depth) ? GL_FLOAT : */GL_UNSIGNED_BYTE,
		(haveImage) ? &image->getBuffer()[0] : nullptr );

	#pragma TODO("Check that there is free memory in the GPU before uploading textures")

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
	if(GLEW_EXT_texture_filter_anisotropic)
		glTexParameterf( target, GL_TEXTURE_MAX_ANISOTROPY_EXT, anisotropicFilter );

	glTexParameteri( target, GL_TEXTURE_MIN_FILTER, convertFilterFormat(minFilter) );
	glTexParameteri( target, GL_TEXTURE_MAG_FILTER, convertFilterFormat(maxFilter) );

	glTexParameterf(target, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameterf(target, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

	//glTexParameter(GL_TEXTURE_BORDER_COLOR);

	// Three next lines are necessary if we want to use the convenient shadow2DProj function in the shader.
	// Otherwise we have to rely on texture2DProj
	 
	// TODO: OpenGL 3 name: COMPARE_REF_TO_TEXTURE
	//glTexParameteri(target, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);
	//glTexParameteri(target, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
	//glTexParameteri(target, GL_DEPTH_TEXTURE_MODE, GL_INTENSITY);

	#pragma TODO("Add support for mipmaps")
}

//-----------------------------------//

void Texture::setImage( Image* newImage )
{
	assert( newImage != nullptr );

	image = newImage;

	width = image->getWidth();
	height = image->getHeight();
	format = image->getPixelFormat();

	upload();
}

//-----------------------------------//

void Texture::bind( int unit ) const
{
	glActiveTexture( GL_TEXTURE0+unit );
	glClientActiveTexture( GL_TEXTURE0+unit );

	glEnable(target);
	glBindTexture( target, id );
}

//-----------------------------------//

void Texture::unbind( int unit ) const
{
	glActiveTexture( GL_TEXTURE0+unit );
	glClientActiveTexture( GL_TEXTURE0+unit );

	glBindTexture( target, 0 );
	glDisable(target);
}

//-----------------------------------//

ImagePtr Texture::readImage() const
{
	std::vector<byte> tmp;
	tmp.resize( getExpectedSize() );

	bind();
	
	glGetTexImage( target, 0 /* base mipmap level */,
		convertSourceFormat(format), GL_UNSIGNED_BYTE, &tmp[0] );
	
	if( glHasError("Could not read texture data") )
		return nullptr;

	unbind();

	ImagePtr image( new Image() );
	image->setWidth( width );
	image->setHeight( height );
	image->setPixelFormat( format );
	image->setBuffer( tmp );

	return image;
}

//-----------------------------------//

uint Texture::getExpectedSize() const
{
	uint size = width*height;

	switch( format )
	{
	case PixelFormat::R8G8B8A8:
		return size*4;
	case PixelFormat::R8G8B8:
		return size*3;
	case PixelFormat::Depth:
		return size;
	}

	assert( 0 && "This should not be reached" );
	return 0;
}

//-----------------------------------//

int Texture::convertFilterFormat( TextureFiltering::Enum format ) const
{
	switch( format )
	{
	case TextureFiltering::Nearest:
		return GL_NEAREST;
	case TextureFiltering::Linear:
		return GL_LINEAR;
	}

	assert( 0 && "This should not be reached" );
	return 0;
}

//-----------------------------------//

GLint Texture::convertInternalFormat( PixelFormat::Enum format ) const
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

	assert( 0 && "This should not be reached" );
	return 0;
}

//-----------------------------------//

GLint Texture::convertSourceFormat( PixelFormat::Enum format ) const
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

	assert( 0 && "This should not be reached" );
	return 0;
}

//-----------------------------------//

} // end namespace