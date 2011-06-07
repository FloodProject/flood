/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Engine/API.h"
#include "Render/TextureManager.h"
#include "Resources/ResourceManager.h"
#include "Render/GL.h"

namespace vapor {

//-----------------------------------//

TextureManager::TextureManager()
{
	ResourceManager* res = GetResourceManager();
	res->onResourceLoaded.Connect( this, &TextureManager::onLoaded );
	res->onResourceRemoved.Connect( this, &TextureManager::onUnloaded );
	res->onResourceReloaded.Connect( this, &TextureManager::onReloaded );
}

//-----------------------------------//

TextureManager::~TextureManager()
{
	ResourceManager* res = GetResourceManager();
	res->onResourceLoaded.Disconnect( this, &TextureManager::onLoaded );
	res->onResourceRemoved.Disconnect( this, &TextureManager::onUnloaded );
	res->onResourceReloaded.Disconnect( this, &TextureManager::onReloaded );

	#pragma TODO("Make sure all textures are released on exit")
	TextureMap::const_iterator it;
	for( it = textures.begin(); it != textures.end(); it++ )
	{
		const TexturePtr& texture = it->second;
		//assert( texture->getReferenceCount() == 2 );
	}
}

//-----------------------------------//

void TextureManager::update( float delta )
{
	//std::vector<TexturePtr> texturesToRemove;

	//foreach( const TextureMapPair& p, textures )
	//{
	//	if( p.second->getReferenceCount() == 2 )
	//	{
	//		texturesToRemove.push_back(p.second);
	//	}
	//}
}

//-----------------------------------//

void TextureManager::removeTexture(Image* image)
{
	TextureMap::iterator it = textures.find(image);
	
	if( it == textures.end() )
		return;

	textures.erase(it);
}

//-----------------------------------//

const uint8 TEX_SIZE = 64;

TexturePtr TextureManager::getTexture( const String& name )
{
	ResourceManager* res = GetResourceManager();
	ImageHandle handle = res->loadResource<Image>(name);
	return getTexture(handle.Resolve());
}

//-----------------------------------//

TexturePtr TextureManager::getTexture( Image* image )
{
	// Image not found.
	if( !image ) 
	{
		//LogWarn( "Reverting to fallback texture" );
		TexturePtr texture = Allocate(Texture, AllocatorGetHeap(), Settings(TEX_SIZE, TEX_SIZE));
		return texture;
	}

	// Image already has texture.
	else if( textures.find(image) != textures.end() )
	{
		return textures[image];
	}

	// Image not loaded yet.
	else if( !image->isLoaded() ) 
	{
		TexturePtr texture = Allocate(Texture, AllocatorGetHeap(), Settings(TEX_SIZE, TEX_SIZE));
		textures[image] = texture;
		return texture;
	}

	// Create a new texture from image.
	else
	{
		TexturePtr texture = Allocate(Texture, AllocatorGetHeap(), image);
		textures[image] = texture;
		return texture;
	}
}

//-----------------------------------//

void TextureManager::onLoaded( const ResourceEvent& event )
{
	ImageHandle handleImage = HandleCast<Image>(event.handle);	
	Image* image = handleImage.Resolve();

	if( image->getResourceGroup() != ResourceGroup::Images )
		return;

	if( textures.find(image) == textures.end() )
		return;

	TexturePtr texture = textures[image];
	texture->setImage(image);
}

//-----------------------------------//

void TextureManager::onUnloaded( const ResourceEvent& event )
{
	ImageHandle handleImage = HandleCast<Image>(event.handle);	
	Image* image = handleImage.Resolve();

	if( image->getResourceGroup() != ResourceGroup::Images )
		return;

	if( textures.find(image) == textures.end() )
		return;

	LogDebug( "Removing texture '%s'", image->getPath().c_str() );

	removeTexture(image);
}

//-----------------------------------//

void TextureManager::onReloaded( const ResourceEvent& event )
{
	ImageHandle handleImage = HandleCast<Image>(event.handle);	
	Image* image = handleImage.Resolve();

	if( image->getResourceGroup() != ResourceGroup::Images )
		return;

	if( textures.find(image) == textures.end() )
		return;

	LogDebug( "Reloading texture '%s'", image->getPath().c_str() );

	TexturePtr tex = textures[image];
	tex->setImage(image);

	//switchImage( currImage, newImage );
}

//-----------------------------------//

void TextureManager::switchImage( const ImagePtr& curr, const ImagePtr& new_ )
{
	TexturePtr texture = textures[curr.get()];
	texture->setImage(new_.get());
	
	textures.erase(curr.get());
	textures[new_.get()] = texture;
}

//-----------------------------------//

uint TextureManager::getMemoryUsage()
{
	uint total = 0;

	TextureMap::const_iterator it;
	for( it = textures.begin(); it != textures.end(); it++ )
		total += it->first->getBuffer().size();

	return total;
}

//-----------------------------------//

} // end namespace