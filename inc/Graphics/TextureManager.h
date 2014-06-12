/************************************************************************
*
* Flood Project � (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

#include "Graphics/Texture.h"
#include "Graphics/Resources/Image.h"

NAMESPACE_GRAPHICS_BEGIN

//-----------------------------------//

class ResourceManager;
struct ResourceEvent;

class RenderBackend;

/**
 * Manages a set of textures. The rest of the engine, when needing a 
 * texture will ask the texture manager for the texture for a given image.
 * This way the engine will be able to provide, for example, fallback 
 * texture when it is not possible to load the requested image file.
 * This will be pretty useful when resource auto-loading is added, if a 
 * given file is corrupted we can safely render the scene, falling back
 * for default textures.
 */

class API_GRAPHICS TextureManager
{
public:

	TextureManager();
	~TextureManager();

	// Gets a texture given a name identifier.
	TexturePtr getTexture( const String& name );

	// Gets a texture given an image.
	TexturePtr getTexture( Image* );

	// Gets a texture given an image handle.
	TexturePtr getTexture( const ImageHandle& handle );

	// Removes the texture backed by the image.
	void removeTexture( Image* );

	// Gets the memory usage of the textures loaded (in kbytes).
	uint32 getMemoryUsage();

	// Sets the rendering backend.
	SETTER(RenderBackend, RenderBackend*, backend)

protected:

	// Populates a texture when the image is loaded.
	void onLoaded( const ResourceEvent& event );

	// Removes a texture when the image file is unloaded.
	void onUnloaded( const ResourceEvent& event );

	// Reloads a texture when the image file changes.
	void onReloaded( const ResourceEvent& event );

	// Maps the identifiers to the textures.
	typedef HashMap< Image*, TexturePtr > TextureMap;
	typedef std::pair< Image*, TexturePtr > TextureMapPair;
	TextureMap textures;

	RenderBackend* backend;
};

//-----------------------------------//

NAMESPACE_GRAPHICS_END