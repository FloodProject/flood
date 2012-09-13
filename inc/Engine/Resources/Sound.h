/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

#include "Resources/Resource.h"
#include "Resources/ResourceLoader.h"

NAMESPACE_ENGINE_BEGIN

//-----------------------------------//

const int SOUND_LOAD_STREAM = 1;

struct SoundStream : public ResourceStream
{
};

//-----------------------------------//

/**
 * Represents a sound resource that holds uncompressed audio data.
 */

REFLECT_DECLARE_CLASS(Sound)

class API_ENGINE Sound : public Resource
{
	REFLECT_DECLARE_OBJECT(Sound)

public:

	Sound();

	// Gets/sets the audio sample frequency.
	ACCESSOR(Frequency, int32, frequency)

	// Gets/sets the audio channels.
	ACCESSOR(Channels, int32, channels)

	// Gets/sets the audio sample size.
	ACCESSOR(Size, int32, size)

	// Gets/sets the audio streaming flag.
	ACCESSOR(Streamed, bool, streamed)

	// Gets/sets the buffer containing the data.
	ACCESSOR(Buffer, const std::vector<byte>&, dataBuffer)

	// Return the proper resource group for this resource.
	GETTER(ResourceGroup, ResourceGroup, ResourceGroup::Audio)

	// Sound frequency.
	int32 frequency;

	// Sound channels.
	int32 channels;

	// Sound sample size.
	int32 size;

	// Use sound streaming.
	bool streamed;

	// Holds the sound data.
	std::vector<byte> dataBuffer;
};

TYPEDEF_RESOURCE_HANDLE_FROM_TYPE( Sound );

//-----------------------------------//

NAMESPACE_ENGINE_END
