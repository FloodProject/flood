/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "CompileOptions.h"

#ifdef VAPOR_IMAGE_STB

#include "Resources/ResourceLoader.h"
#include "Resources/Image.h"

namespace vapor {

//-----------------------------------//

/**
 * This codec provides image decoding services using stb_image.
 */

class STB_Image_Loader : public ResourceLoader
{
	DECLARE_CLASS_()

public:

	STB_Image_Loader();

	// Creates the resource with no data.
	RESOURCE_LOADER_PREPARE(Image)

	// Decode an image file to a buffer.
	virtual bool decode(const Stream& file, Resource* res);

	// Gets the name of this codec.
	GETTER(Name, const std::string, "STB_IMAGE")

	// Overrides this to return the right resource group.
	GETTER(ResourceGroup, ResourceGroup::Enum, ResourceGroup::Images)
};

//-----------------------------------//

} // end namespace

#endif