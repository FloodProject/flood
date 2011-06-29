/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#ifdef ENABLE_AUDIO_OGG

#include "Resources/ResourceLoader.h"
#include "Resources/Sound.h"

#ifdef COMPILER_MSVC
	#pragma warning(disable : 4244 )
#endif

#include <vorbis/vorbisfile.h>

namespace vapor {

//-----------------------------------//

/**
 * Loader for OGG Vorbis files.
 */

REFLECT_DECLARE_CLASS(OGG_Loader)

class RESOURCE_API OGG_Loader : public ResourceLoader
{
	REFLECT_DECLARE_OBJECT(OGG_Loader)

public:

	OGG_Loader();

	// Creates the resource with no data.
	RESOURCE_LOADER_PREPARE(Sound)

	// Gets the class of the resource.
	RESOURCE_LOADER_CLASS(Sound)

	// Decode an OGG file to a buffer.
	bool decode(const Stream& file, Resource* res) OVERRIDE;

	// Gets the name of this codec.
	GETTER(Name, const String, "OGG")

	// Overrides this to return the right resource group.
	GETTER(ResourceGroup, ResourceGroup::Enum, ResourceGroup::Audio)

protected:

	void decodeOgg( OggVorbis_File* vf, std::vector<byte>& buffer );

	// Used for providing libvorbisfile with I/O callbacks.
	ov_callbacks callbacks;
};

//-----------------------------------//

} // end namespace

#endif