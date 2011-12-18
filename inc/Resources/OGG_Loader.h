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

NAMESPACE_RESOURCES_BEGIN

//-----------------------------------//

/**
 * Loader for OGG Vorbis files.
 */

REFLECT_DECLARE_CLASS(OGG_Loader)

class API_RESOURCE OGG_Loader : public ResourceLoader
{
	REFLECT_DECLARE_OBJECT(OGG_Loader)

public:

	OGG_Loader();

	// Creates the resource with no data.
	Resource* prepare(ResourceLoadOptions&) OVERRIDE;

	// Gets the class of the resource.
	RESOURCE_LOADER_CLASS(Sound)

	// Decode an OGG file to a buffer.
	bool decode(ResourceLoadOptions&) OVERRIDE;

	// Gets the name of this codec.
	GETTER(Name, const String, "OGG")

	// Overrides this to return the right resource group.
	GETTER(ResourceGroup, ResourceGroup::Enum, ResourceGroup::Audio)

protected:

	// Initializes the sound with the Ogg info.
	bool initOgg( OggVorbis_File*&, ResourceLoadOptions& );

	// Decodes the audio from the Ogg stream.
	void decodeOgg( OggVorbis_File* vf, std::vector<byte>& buffer );

	// Used for providing libvorbisfile with I/O callbacks.
	ov_callbacks callbacks;
};

//-----------------------------------//

NAMESPACE_RESOURCES_END

#endif