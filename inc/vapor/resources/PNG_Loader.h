/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "vapor/CompileOptions.h"

#ifdef VAPOR_IMAGE_PICOPNG

#include "vapor/resources/ResourceLoader.h"
#include "vapor/resources/Image.h"

namespace vapor {

//-----------------------------------//

/**
 * This codec provides PNG decoding services using the lightweight, 
 * thin, tiny, awesome, picoPNG library. :)
 */

class PNG_Pico_Loader : public ResourceLoader
{
public:

	PNG_Pico_Loader();
	
	// Creates the resource with no data.
	IMPLEMENT_PREPARE(Image)

	// Decode an image file to a buffer.
	virtual bool decode(const File&, Resource* res);

	// Gets the name of this codec.
	IMPLEMENT_GETTER(Name, const std::string, "PICOPNG")

	// Gets the list of extensions this codec can handle.
	IMPLEMENT_GETTER(Extensions, ExtensionList&, extensions)

	// Overrides this to return the right resource group.
	IMPLEMENT_GETTER(ResourceGroup, ResourceGroup::Enum, ResourceGroup::Images)

protected:

	// Flip the Y part of the texture. OpenGL expects the origin at 
	// bottom-left while the PNG gets decoded as origin being top-left.
	void flip( std::vector<byte>& buffer, ulong width, ulong height );

	// Holds all file extensions recognized by this codec.
	mutable ExtensionList extensions;
};

//-----------------------------------//

} // end namespace

#endif