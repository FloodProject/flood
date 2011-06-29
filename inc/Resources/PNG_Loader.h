/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#ifdef ENABLE_IMAGE_PICOPNG

#include "Resources/ResourceLoader.h"
#include "Resources/Image.h"

namespace vapor {

//-----------------------------------//

/**
 * This codec provides PNG decoding services using the lightweight, 
 * thin, tiny, awesome, picoPNG library. :)
 */

REFLECT_DECLARE_CLASS(PNG_Pico_Loader)

class PNG_Pico_Loader : public ResourceLoader
{
	REFLECT_DECLARE_OBJECT(PNG_Pico_Loader)

public:

	PNG_Pico_Loader();
	
	// Creates the resource with no data.
	RESOURCE_LOADER_PREPARE(Image)

	// Gets the class of the resource.
	RESOURCE_LOADER_CLASS(Image)

	// Decode an image file to a buffer.
	virtual bool decode(const Stream& file, Resource* res) OVERRIDE;

	// Gets the name of this codec.
	GETTER(Name, const String, "PICOPNG")

	// Overrides this to return the right resource group.
	GETTER(ResourceGroup, ResourceGroup::Enum, ResourceGroup::Images)

	bool flipAxisY;

protected:

	// Flip the Y part of the texture. OpenGL expects the origin at 
	// bottom-left while the PNG gets decoded as origin being top-left.
	void flip( std::vector<byte>& buffer, uint32 width, uint32 height );
};

//-----------------------------------//

} // end namespace

#endif