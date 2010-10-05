/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "vapor/PCH.h"
#include "vapor/render/TextureManager.h"
#include "vapor/resources/ResourceManager.h"
#include "vapor/render/GL.h"

namespace vapor {

//-----------------------------------//

TextureManager::TextureManager( ResourceManager* resourceManager )
	: rm( resourceManager )
{
	assert( rm != nullptr );
	
	rm->onResourceLoaded += fd::bind( &TextureManager::onLoad, this );
	rm->onResourceReloaded += fd::bind( &TextureManager::onReload, this );
}

//-----------------------------------//

TextureManager::~TextureManager()
{
	rm->onResourceLoaded -= fd::bind( &TextureManager::onLoad, this );
	rm->onResourceReloaded -= fd::bind( &TextureManager::onReload, this );

	#pragma TODO("Make sure all textures are released on exit")
	//foreach( const TextureMapPair& p, textures )
	//	assert( p.second->getReferenceCount() == 2 );
}

//-----------------------------------//

const byte TEX_SIZE = 64;

TexturePtr TextureManager::getTexture( const std::string& name )
{
	ImagePtr img = rm->loadResource<Image>(name);
	return getTexture(img);
}

//-----------------------------------//

TexturePtr TextureManager::getTexture( const ImagePtr& img )
{
	// Image not found.
	if( !img ) 
	{
		//warn( "render", "Reverting to fallback texture" );
		return TexturePtr( new Texture( Settings(TEX_SIZE, TEX_SIZE) ) );
	}

	// Image already has texture.
	else if( textures.find(img) != textures.end() )
	{
		return textures[img];
	}

	// Image not loaded yet.
	else if( !img->isLoaded() ) 
	{
		TexturePtr tex( new Texture( Settings(TEX_SIZE, TEX_SIZE) ) );
		textures[img] = tex;
		return tex;
	}

	// Create a new texture from image.
	else
	{
		TexturePtr tex( new Texture(img) );
		textures[img] = tex;
		return tex;
	}
}

//-----------------------------------//

void TextureManager::onLoad( const ResourceEvent& evt )
{
	if( evt.resource->getResourceGroup() != ResourceGroup::Images )
		return;
	
	const ImagePtr& image = boost::static_pointer_cast<Image>( evt.resource );

	if( textures.find(image) == textures.end() )
		return;

	TexturePtr texture = textures[image];
	texture->setImage(image);
}

//-----------------------------------//

void TextureManager::onReload( const ResourceEvent& evt )
{
	if( evt.resource->getResourceGroup() != ResourceGroup::Images )
		return;

	const ImagePtr& img = boost::static_pointer_cast<Image>( evt.resource );
	//const ImagePtr& newImage = RESOURCE_SMART_PTR_CAST<Image>( evt.newResource );

	if( textures.find(img) == textures.end() )
		return;

	debug( "Reloading texture '%s'", evt.resource->getPath().c_str() );

	TexturePtr tex = textures[img];
	tex->setImage(img);

	//switchImage( currImage, newImage );
}

//-----------------------------------//

void TextureManager::switchImage( const ImagePtr& curr, const ImagePtr& new_ )
{
	TexturePtr texture = textures[curr];
	texture->setImage(new_);
	
	textures.erase(curr);
	textures[new_] = texture;
}

//-----------------------------------//

uint TextureManager::getMemoryUsage()
{
	uint total = 0;

	foreach( const TextureMapPair& p, textures )
		total += p.first->getBuffer().size();

	return total;
}

//-----------------------------------//

} // end namespace