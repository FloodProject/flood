/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#ifdef ENABLE_IMAGE_DEVIL

#include "Resources/ResourceLoader.h"
#include "Resources/Image.h"

namespace vapor {

//-----------------------------------//

/**
 * This codec provides image decoding services using stb_image.
 */

REFLECT_DECLARE_CLASS(IL_Image_Loader)

class IL_Image_Loader : public ResourceLoader
{
	REFLECT_DECLARE_OBJECT(IL_Image_Loader)

public:

	IL_Image_Loader();
	~IL_Image_Loader();

	// Creates the resource with no data.
	RESOURCE_LOADER_PREPARE(Image)

	// Decode an image file to a buffer.
	virtual bool decode(const Stream& file, Resource* res);

	// Gets the name of this codec.
	GETTER(Name, const String, "STB_IMAGE")

	// Overrides this to return the right resource group.
	GETTER(ResourceGroup, ResourceGroup::Enum, ResourceGroup::Images)

	// Checks for loading errors.
	bool checkErrors();
};

//-----------------------------------//

} // end namespace

#endif