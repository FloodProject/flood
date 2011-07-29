/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "Resources/Resource.h"

NAMESPACE_BEGIN

//-----------------------------------//

/**
 * Represents a sound resource that holds uncompressed audio data.
 */

REFLECT_DECLARE_CLASS(Sound)

class API_RESOURCE Sound : public Resource
{
	REFLECT_DECLARE_OBJECT(Sound)

public:

	Sound();

	// Gets/sets the audio sample frequency.
	ACESSOR(Frequency, int32, frequency)

	// Gets/sets the audio channels.
	ACESSOR(Channels, int32, channels)

	// Gets/sets the audio sample size.
	ACESSOR(Size, int32, size)

	// Gets/sets the buffer containing the data.
	ACESSOR(Buffer, const std::vector<byte>&, dataBuffer)

	// Return the proper resource group for this resource.
	GETTER(ResourceGroup, ResourceGroup::Enum, ResourceGroup::Audio)

	// Sound frequency.
	int32 frequency;

	// Sound channels.
	int32 channels;

	// Sound sample size.
	int32 size;

	// Holds the sound data.
	std::vector<byte> dataBuffer;
};

TYPEDEF_RESOURCE_HANDLE_FROM_TYPE( Sound );

//-----------------------------------//

NAMESPACE_END