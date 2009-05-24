/************************************************************************
*
* vaporEngine by triton © (2008-2009)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#pragma once

#include "vapor/CompileOptions.h"

#ifdef VAPOR_IMAGE_CODEC_PICOPNG

#include "vapor/resources/ResourceLoader.h"
#include "vapor/resources/Image.h"

namespace vapor {
	namespace resources {

/**
 * This codec provides PNG decoding services using the lightweight, 
 * thin, tiny, awesome, picoPNG library. :)
 */

class PNG_Pico_Loader : public ResourceLoader
{
public:

	PNG_Pico_Loader();

	// Decode an image file to a buffer
	virtual Image* decode(File& image);

	// Gets the name of this codec.
	virtual string getName() { return "PICOPNG"; }

	// Gets the list of extensions this codec can handle.
	virtual list<string>& getExtensions() { return extensions; }

	// Overrides this to return the right resource group.
	virtual ResourceGroup::Enum getResourceGroup() { return ResourceGroup::Images; }

protected:

	// holds all file extensions recognized by this codec
	list<string> extensions;
};

} } // end namespaces

#endif