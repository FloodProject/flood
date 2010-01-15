/************************************************************************
*
* vaporEngine (2008-2010)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#pragma once

#include "vapor/CompileOptions.h"

#ifdef VAPOR_IMAGE_PICOPNG

#include "vapor/resources/ResourceLoader.h"
#include "vapor/resources/Image.h"

namespace vapor { namespace resources {

//-----------------------------------//

/**
 * This codec provides PNG decoding services using the lightweight, 
 * thin, tiny, awesome, picoPNG library. :)
 */

class PNG_Pico_Loader : public ResourceLoader
{
public:

	PNG_Pico_Loader();

	// Decode an image file to a buffer.
	virtual Image* decode(const vfs::File& file);

	// Gets the name of this codec.
	virtual const std::string getName() { return "PICOPNG"; }

	// Gets the list of extensions this codec can handle.
	virtual 
		std::list< std::string >& getExtensions() { return extensions; }

	// Overrides this to return the right resource group.
	virtual ResourceGroup::Enum getResourceGroup() { return ResourceGroup::Images; }

protected:

	// Flip the Y part of the texture. OpenGL expects the origin at 
	// bottom-left while the PNG gets decoded as origin being top-left.
	void flip( std::vector<byte>& buffer, ulong width, ulong height );

	// Holds all file extensions recognized by this codec.
	std::list< std::string > extensions;
};

//-----------------------------------//

} } // end namespaces

#endif