/************************************************************************
*
* vaporEngine (2008-2009)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#include "vapor/PCH.h"

#include "vapor/render/TextureManager.h"

using namespace vapor::resources;

namespace vapor { namespace render {

//-----------------------------------//

TextureManager::TextureManager()
{

}

//-----------------------------------//

TextureManager::~TextureManager()
{

}

//-----------------------------------//

TexturePtr TextureManager::getTexture( const std::string& name )
{
	ImagePtr img = ResourceManager::getInstancePtr()->loadResource<Image>( name );

	if( !img )
	{
		// use a fallback default texture
		// TODO: bundle fallback texture in engine
		
		warn( "render::textures", 
			"Could not locate '%s': reverting to fallback texture", name.c_str() );

		img = ResourceManager::getInstancePtr()->loadResource<Image>( "media/fallback.png" );
	}

	return getTexture(img);
}

//-----------------------------------//

TexturePtr TextureManager::getTexture( ImagePtr img )
{
 	if( textures.find( img ) != textures.end() )
	{
		// we have already a texture for this image
		return textures[img];
	}

	TexturePtr tex( new Texture( img ) );
	textures[img] = tex;
	return tex;
}

//-----------------------------------//

uint TextureManager::getMemoryUsage()
{
	uint total = 0;

	foreach( texPair p, textures )
		total += p.first->getBuffer().size();

	return total;
}

//-----------------------------------//

} } // end namespaces