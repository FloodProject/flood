/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#ifdef VAPOR_AUDIO_OGG

#include "vapor/resources/ResourceLoader.h"
#include "vapor/resources/Sound.h"

#include <vorbis/vorbisfile.h>

namespace vapor {

//-----------------------------------//

/**
 * Loader for OGG Vorbis files.
 */

class OGG_Loader : public ResourceLoader
{
public:

	OGG_Loader();

	// Creates the resource with no data.
	PREPARE(Sound)

	// Decode an OGG file to a buffer.
	virtual bool decode(const File&, Resource* res);

	// Gets the name of this codec.
	GETTER(Name, const std::string, "OGG")

	// Gets the list of extensions this codec can handle.
	GETTER(Extensions, ExtensionList&, extensions)

	// Overrides this to return the right resource group.
	GETTER(ResourceGroup, ResourceGroup::Enum, ResourceGroup::Audio)

protected:

	void decodeOgg( OggVorbis_File* vf, std::vector<byte>& buffer );

	// holds all file extensions recognized by this codec
	mutable ExtensionList extensions;

	// Used for providing libvorbisfile with I/O callbacks.
	ov_callbacks callbacks;
};

//-----------------------------------//

} // end namespace

#endif