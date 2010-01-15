/************************************************************************
*
* vaporEngine (2008-2010)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#pragma once

#include "vapor/Platform.h"
#include "vapor/Singleton.h"

#include "vapor/resources/ResourceManager.h"
#include "vapor/resources/Image.h"

#include "vapor/render/Texture.h"
#include "vapor/render/GL.h"

namespace vapor { namespace render {

//-----------------------------------//

/**
 * Manages a set of textures. The rest of the engine, when needing a 
 * texture will ask the texture manager for the texture for a given class.
 * This way the engine will be able to provide, for example, fallback 
 * textures for when it is not possible to load the requested image file.
 * This will be pretty useful when resource auto-loading is added, if a 
 * given file is corrupted we can safely render the scene, falling back
 * for default textures.
 */

class VAPOR_API TextureManager : public Singleton<TextureManager>
{
public:

	TextureManager();
	~TextureManager();

	// Gets a texture given a name identifier.
	TexturePtr getTexture( const std::string& tex );

	// Gets a texture given an image.
	TexturePtr getTexture( resources::ImagePtr img );

	// Gets the memory usage of the textures loaded (in kbytes).
	uint getMemoryUsage();

private:

	// Maps the identifiers to the textures.
	std::map< resources::ImagePtr, TexturePtr > textures;
	typedef std::pair< resources::ImagePtr, TexturePtr > texPair;
	
};

//-----------------------------------//

} } // end namespaces