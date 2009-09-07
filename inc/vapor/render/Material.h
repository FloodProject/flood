/************************************************************************
*
* vaporEngine (2008-2009)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#pragma once

#include "vapor/Platform.h"

#include "vapor/render/Texture.h"

namespace vapor {
	namespace render {

//-----------------------------------//

/**
 * Rendering material.
 * TODO: Serialization?
 */

class Material
{
public:

	Material( const std::string& name );
	~Material();

	const std::string& getName();

	void addTexture( TexturePtr tex );

private:

	// Textures
	std::vector< TexturePtr > textures;

	// Shaders

	std::string name;

};

//-----------------------------------//

typedef tr1::shared_ptr< Material > MaterialPtr;

//-----------------------------------//

} } // end namespaces