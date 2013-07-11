/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#include "Graphics/API.h"
#include "Graphics/TextureManager.h"
#include "Graphics/RenderBackend.h"
#include "Resources/ResourceManager.h"

#include "Core/Containers/Hash.h"

NAMESPACE_GRAPHICS_BEGIN

//-----------------------------------//

TextureManager::TextureManager()
	: textures(*AllocatorGetHeap())
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
}

//-----------------------------------//

void TextureManager::removeTexture(Image* image)
{
	if(hash::has(textures, (uint64)image))
		hash::remove(textures, (uint64)image);
}

//-----------------------------------//

static const uint8 TEX_SIZE = 64;

TexturePtr TextureManager::getTexture( const String& name )
{
	ResourceManager* res = GetResourceManager();
	ImageHandle handle = res->loadResource<Image>(name);
	return getTexture(handle);
}

//-----------------------------------//

TexturePtr TextureManager::getTexture( Image* image )
{
	if( !image )
		return nullptr;

	auto tex = hash::get<TexturePtr>(textures, (uint64)image, nullptr);
	
	if( !tex && !image->isLoaded())
	{
		tex = backend->createTexture();
		tex->allocate(Vector2i(TEX_SIZE, TEX_SIZE), PixelFormat::R8G8B8A8);

		hash::set(textures, (uint64)image, tex);
	}

	return tex;
}

//-----------------------------------//

TexturePtr TextureManager::getTexture( const ImageHandle& imageHandle )
{
	auto image = imageHandle.Resolve();
	auto tex = getTexture(image);
	if (!tex)
	{
		// Create a new texture from image.
		tex = backend->createTexture();
		tex->setImage(imageHandle);

		hash::set(textures, (uint64)image, tex);
	}

	return tex;
}

//-----------------------------------//

void TextureManager::onLoaded( const ResourceEvent& event )
{
	ImageHandle handleImage = HandleCast<Image>(event.handle);
	Image* image = handleImage.Resolve();

	if( image->getResourceGroup() != ResourceGroup::Images )
		return;

	auto tex = (hash::get<TexturePtr>(textures, (uint64)image, nullptr)).get();
	if(tex)
	{
		tex->setImage(handleImage);
		backend->uploadTexture(tex);
	}
}

//-----------------------------------//

void TextureManager::onUnloaded( const ResourceEvent& event )
{
	ImageHandle handleImage = HandleCast<Image>(event.handle);
	Image* image = handleImage.Resolve();

	if( image->getResourceGroup() != ResourceGroup::Images )
		return;

	if(!hash::has(textures, (uint64)image))
		return;

	LogDebug( "Removing texture '%s'", image->getPath().c_str() );

	removeTexture(image);
}

//-----------------------------------//

void TextureManager::onReloaded( const ResourceEvent& event )
{
	ImageHandle handleImage = HandleCast<Image>(event.handle);
	Image* newImage = handleImage.Resolve();

	if( newImage->getResourceGroup() != ResourceGroup::Images )
		return;
	
	Image* oldImage = (Image*) event.oldResource;

	if(!hash::has(textures, (uint64)oldImage))
		return;

	LogDebug( "Reloading texture '%s'", newImage->getPath().c_str() );

	auto tex = hash::get<TexturePtr>(textures, (uint64)oldImage, nullptr);
	tex->setImage(handleImage);

	hash::remove(textures, (uint64)oldImage);
	hash::set(textures, (uint64)newImage, tex);
}

//-----------------------------------//

uint TextureManager::getMemoryUsage()
{
	uint total = 0;

	for(auto it : textures)
	{
		auto img = (Image*)it.key;
		total += array::size(img->getBuffer());
	}

	return total;
}

//-----------------------------------//

NAMESPACE_GRAPHICS_END