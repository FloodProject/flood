/************************************************************************
*
* vaporEngine (2008-2009)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#pragma once

#include "vapor/CompileOptions.h"

#ifdef VAPOR_AUDIO_OGG

#include "vapor/resources/ResourceLoader.h"
#include "vapor/resources/Sound.h"

#include <vorbis/vorbisfile.h>

namespace vapor {
	namespace resources {

/**
 * Loader for OGG Vorbis files.
 */

class OGG_Loader : public ResourceLoader
{
public:

	OGG_Loader();

	// Decode an OGG file to a buffer.
	virtual Sound* decode(const vfs::File& file);

	// Gets the name of this codec.
	virtual const std::string getName() { return "OGG"; }

	// Gets the list of extensions this codec can handle.
	virtual std::list< std::string >& getExtensions() { return extensions; }

	// Overrides this to return the right resource group.
	virtual ResourceGroup::Enum getResourceGroup() { return ResourceGroup::Audio; }

protected:

	// holds all file extensions recognized by this codec
	std::list< std::string > extensions;

	// Used for providing libvorbisfile with I/O callbacks.
	ov_callbacks callbacks;
};

} } // end namespaces

#endif