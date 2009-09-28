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

namespace vapor {
	namespace render {

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
	ImagePtr img = ResourceManager::getInstance()->loadResource<Image>( name );

	if( !img )
	{
		// use a fallback default texture
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

} } // end namespaces