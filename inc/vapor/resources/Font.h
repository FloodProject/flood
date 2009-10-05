/************************************************************************
*
* vaporEngine (2008-2009)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#pragma once

#include "vapor/Platform.h"
#include "vapor/resources/Resource.h"

namespace vapor {
	namespace resources {

//-----------------------------------//

class VAPOR_API Font : public Resource
{
public:

	Font();
	virtual ~Font();

	// Gets the texture that backs this font.
	//TexturePtr getTexture();

	// Gets the renderable that backs this font.
	//RenderablePtr getRenderable();

	virtual ResourceGroup::Enum getResourceGroup() const;



protected:


};

//-----------------------------------//

} } // end namespaces
