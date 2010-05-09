/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "vapor/PCH.h"
#include "vapor/render/GL.h"
#include "vapor/render/Texture.h"
#include "vapor/render/Adapter.h"
#include "vapor/render/Device.h"
#include "vapor/Engine.h"

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
	target = GL_TEXTURE_2D;

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
	// TODO: refactor
	AdapterPtr adapter = Engine::getInstance().getRenderDevice()->getAdapter();
	uint max = adapter->getMaxTextureSize();

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

	glTexImage2D( target, 0, convertInternalFormat(format),
		width, height, 0, convertSourceFormat(format),
		format == PixelFormat::Depth ? GL_FLOAT : GL_UNSIGNED_BYTE, image ? &image->getBuffer()[0] : nullptr );

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
	glTexParameterf(target, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(target, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glTexParameterf(target, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(target, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	// Three next lines are necessary if we wan to use the convenient shadow2DProj function in the shader.
	// Otherwise we have to rely on texture2DProj
	 
	// TODO: OpenGL 3 name: COMPARE_REF_TO_TEXTURE
	//glTexParameteri(target, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);
	//glTexParameteri(target, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
	//glTexParameteri(target, GL_DEPTH_TEXTURE_MODE, GL_INTENSITY);

	//glTexParameterf(target,GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	//glTexParameterf(target,GL_TEXTURE_MIN_FILTER, GL_LINEAR );

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

void Texture::bind( int unit ) const
{
	glActiveTexture( GL_TEXTURE0+unit );
	glBindTexture( target, _id );
}

//-----------------------------------//

void Texture::unbind( int unit ) const
{
	glActiveTexture( GL_TEXTURE0+unit );
	glBindTexture( target, 0 );
}

//-----------------------------------//

ImagePtr Texture::readImage() const
{
	std::vector<float> tmp;
	tmp.resize( getExpectedSize() );

	bind();
	
	glGetTexImage( target, 0 /* base mipmap level */,
		convertGetFormat(format), GL_FLOAT, &tmp[0] );
	
	if( glHasError("Could not read texture data") )
		return ImagePtr();

	unbind();

	std::vector<byte> data;
	data.reserve( getExpectedSize() );
	
	foreach( float& f, tmp )
		data.push_back( f*255 );
	
	ImagePtr image( new Image() );
	image->setWidth( width );
	image->setHeight( height );
	image->setPixelFormat( format );
	image->setBuffer( data );
	image->setStatus( ResourceStatus::Loaded );

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

	assert( 0 && "Implement support for more pixel formats" );
	return 0;
}

//-----------------------------------//

GLint Texture::convertGetFormat( PixelFormat::Enum format ) const
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

	assert( 0 && "Implement support for more pixel formats" );
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