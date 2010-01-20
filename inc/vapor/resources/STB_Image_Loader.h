/************************************************************************
*
* vaporEngine (2008-2010)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#pragma once

#include "vapor/CompileOptions.h"

#ifdef VAPOR_IMAGE_STB

#include "vapor/resources/ResourceLoader.h"
#include "vapor/resources/Image.h"

namespace vapor { namespace resources {

//-----------------------------------//

/**
 * This codec provides image decoding services using stb_image.
 */

class STB_Image_Loader : public ResourceLoader
{
public:

	STB_Image_Loader();

	// Decode an image file to a buffer.
	virtual Image* decode(const vfs::File& file);

	// Gets the name of this codec.
	virtual const std::string getName() { return "STB_IMAGE"; }

	// Gets the list of extensions this codec can handle.
	virtual std::list< std::string >& getExtensions() { return extensions; }

	// Overrides this to return the right resource group.
	virtual ResourceGroup::Enum getResourceGroup() { return ResourceGroup::Images; }

protected:

	// Holds all file extensions recognized by this codec.
	std::list< std::string > extensions;
};

//-----------------------------------//

} } // end namespaces

#endif