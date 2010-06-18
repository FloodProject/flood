/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "vapor/resources/Image.h"
#include "vapor/render/Texture.h"

namespace vapor {

class ResourceManager;
struct ResourceEvent;

//-----------------------------------//

/**
 * Manages a set of textures. The rest of the engine, when needing a 
 * texture will ask the texture manager for the texture for a given image.
 * This way the engine will be able to provide, for example, fallback 
 * texture when it is not possible to load the requested image file.
 * This will be pretty useful when resource auto-loading is added, if a 
 * given file is corrupted we can safely render the scene, falling back
 * for default textures.
 */

class VAPOR_API TextureManager
{
public:

	TextureManager( ResourceManager* );
	~TextureManager();

	// Gets a texture given a name identifier.
	TexturePtr getTexture( const std::string& tex );

	// Gets a texture given an image.
	TexturePtr getTexture( const ImagePtr& img );

	// Gets the memory usage of the textures loaded (in kbytes).
	uint getMemoryUsage();

private:

	// Populates a texture when the image is loaded.
	void onLoad( const ResourceEvent& evt );

	// Reloads a texture when the image file changes.
	void onReload( const ResourceEvent& evt );

	// Switches the image of the texture backed by the first image.
	void switchImage( const ImagePtr&, const ImagePtr& );

	// Maps the identifiers to the textures.
	typedef std::map< ImagePtr, TexturePtr > TextureMap;
	typedef std::pair< ImagePtr, TexturePtr > TextureMapPair;
	TextureMap textures;
	
	ResourceManager* rm;
};

//-----------------------------------//

} // end namespace