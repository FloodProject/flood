/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "vapor/PCH.h"
#include "render/TextureManager.h"
#include "resources/ResourceManager.h"
#include "render/GL.h"
#include "Engine.h"

namespace vapor {

//-----------------------------------//

TextureManager::TextureManager()
	: rm( GetEngine()->getResourceManager() )
{
	assert( rm != nullptr );
	
	rm->onResourceLoaded.Connect( this, &TextureManager::onLoaded );
	rm->onResourceRemoved.Connect( this, &TextureManager::onUnloaded );
	rm->onResourceReloaded.Connect( this, &TextureManager::onReloaded );
}

//-----------------------------------//

TextureManager::~TextureManager()
{
	rm->onResourceLoaded.Disconnect( this, &TextureManager::onLoaded );
	rm->onResourceRemoved.Disconnect( this, &TextureManager::onUnloaded );
	rm->onResourceReloaded.Disconnect( this, &TextureManager::onReloaded );

	#pragma TODO("Make sure all textures are released on exit")
	TextureMap::const_iterator it;
	for( it = textures.cbegin(); it != textures.cend(); it++ )
	{
		const TexturePtr& texture = it->second;
		//assert( texture->getReferenceCount() == 2 );
	}
}

//-----------------------------------//

void TextureManager::update( double delta )
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

void TextureManager::removeTexture(const ImagePtr& image)
{
	TextureMap::iterator it = textures.find(image.get());
	
	if( it == textures.end() )
		return;

	textures.erase(it);
}

//-----------------------------------//

const byte TEX_SIZE = 64;

TexturePtr TextureManager::getTexture( const std::string& name )
{
	ImagePtr img = rm->loadResource<Image>(name);
	return getTexture(img);
}

//-----------------------------------//

TexturePtr TextureManager::getTexture( const ImagePtr& image )
{
	Image* img = image.get();

	// Image not found.
	if( !img ) 
	{
		//Log::warn( "Reverting to fallback texture" );
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

void TextureManager::onLoaded( const ResourceEvent& evt )
{
	if( evt.resource->getResourceGroup() != ResourceGroup::Images )
		return;
	
	const ImagePtr& image = RefCast<Image>(evt.resource);

	if( textures.find(image.get()) == textures.end() )
		return;

	TexturePtr texture = textures[image.get()];
	texture->setImage(image.get());
}

//-----------------------------------//

void TextureManager::onUnloaded( const ResourceEvent& evt )
{
	if( evt.resource->getResourceGroup() != ResourceGroup::Images )
		return;

	const ImagePtr& image = RefCast<Image>(evt.resource);

	if( textures.find(image.get()) == textures.end() )
		return;

	Log::debug( "Removing texture '%s'", evt.resource->getPath().c_str() );

	removeTexture(image.get());
}

//-----------------------------------//

void TextureManager::onReloaded( const ResourceEvent& evt )
{
	if( evt.resource->getResourceGroup() != ResourceGroup::Images )
		return;

	const ImagePtr& image = RefCast<Image>(evt.resource);
	//const ImagePtr& newImage = RESOURCE_SMART_PTR_CAST<Image>( evt.newResource );

	if( textures.find(image.get()) == textures.end() )
		return;

	Log::debug( "Reloading texture '%s'", evt.resource->getPath().c_str() );

	TexturePtr tex = textures[image.get()];
	tex->setImage(image.get());

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
	for( it = textures.cbegin(); it != textures.cend(); it++ )
		total += it->first->getBuffer().size();

	return total;
}

//-----------------------------------//

} // end namespace