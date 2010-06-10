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
#include "vapor/Engine.h"

namespace vapor {

//-----------------------------------//

TextureManager::TextureManager()
	: rm( Engine::getInstance().getResourceManager() )
{
	assert( rm != nullptr );
	
	rm->onResourceLoaded += fd::bind( &TextureManager::onLoad, this );
	rm->onResourceReloaded += fd::bind( &TextureManager::onReload, this );
}

//-----------------------------------//

TextureManager::~TextureManager()
{
	assert( rm != nullptr );
	
	rm->onResourceLoaded -= fd::bind( &TextureManager::onLoad, this );
	rm->onResourceReloaded -= fd::bind( &TextureManager::onReload, this );

	foreach( const TextureMapPair& p, textures )
		assert( p.second->getReferenceCount() == 2 );
}

//-----------------------------------//

const byte TEX_SIZE = 64;

TexturePtr TextureManager::getTexture( const std::string& name )
{
	ImagePtr img = rm->loadResource<Image>(name);

	if( !img )
		warn( "render", "Could not find image: '%s'", name.c_str() );

	return getTexture(img);
}

//-----------------------------------//

TexturePtr TextureManager::getTexture( const ImagePtr& img )
{
	// Image not found.
	if( !img ) 
	{
		warn( "render", "Reverting to fallback texture" );
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
	
	const ImagePtr& image = RESOURCE_SMART_PTR_CAST<Image>( evt.resource );

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

	const ImagePtr& img = RESOURCE_SMART_PTR_CAST<Image>( evt.resource );
	//const ImagePtr& newImage = RESOURCE_SMART_PTR_CAST<Image>( evt.newResource );

	if( textures.find(img) == textures.end() )
		return;

	TexturePtr tex = textures[img];
	tex->setImage(img);

	//switchImage( currImage, newImage );

	// Reload the texture.
	debug( "Reloading texture" );
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