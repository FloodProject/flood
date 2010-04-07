/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "vapor/PCH.h"
#include "vapor/render/TextureManager.h"

using namespace vapor::resources;

namespace vapor { namespace render {

//-----------------------------------//

TextureManager::TextureManager()
{
	ResourceManager::getInstance().onResourceReloaded +=
		fd::bind( &TextureManager::onReload, this );
}

//-----------------------------------//

void TextureManager::onReload( const ResourceEvent& evt )
{
	if( evt.resource->getResourceGroup() != ResourceGroup::Images )
		return;

	ImagePtr currImage = RESOURCE_SMART_PTR_CAST<Image>( evt.resource );
	ImagePtr newImage = RESOURCE_SMART_PTR_CAST<Image>( evt.newResource );

	if( textures.find( currImage ) == textures.end() )
		return;

	TexturePtr texture = textures[currImage];

	texture->setImage(newImage);
	texture->bind();
	texture->upload();
	texture->unbind();

	textures.erase( currImage );
	textures[newImage] = texture;

	// Reload the texture.
	debug( "Reloading texture" );
}

//-----------------------------------//

TexturePtr TextureManager::getTexture( const std::string& name )
{
	ResourceManager* rm = ResourceManager::getInstancePtr();
	ImagePtr img = rm->loadResource<Image>( name );

	if( !img )
	{
		// use a fallback default texture
		// TODO: bundle fallback texture in engine
		
		warn( "render::textures", 
			"Could not locate '%s': reverting to fallback texture", name.c_str() );

		img = rm->loadResource<Image>( "fallback.png" );
	}

	return getTexture(img);
}

//-----------------------------------//

TexturePtr TextureManager::getTexture( const ImagePtr& img )
{
 	if( textures.find( img ) != textures.end() )
	{
		// Already have a texture for this image.
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

	foreach( const texPair& p, textures )
		total += p.first->getBuffer().size();

	return total;
}

//-----------------------------------//

} } // end namespaces