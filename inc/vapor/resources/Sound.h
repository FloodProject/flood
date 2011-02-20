/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "Resources/Resource.h"

namespace vapor {

//-----------------------------------//

/**
 * Represents a sound resource that holds uncompressed audio data.
 */

class RESOURCE_API Sound : public Resource
{
	DECLARE_CLASS_()

public:

	Sound();

	// Gets/sets the audio sample frequency.
	ACESSOR(Frequency, const int, frequency)

	// Gets/sets the audio channels.
	ACESSOR(Channels, int, channels)

	// Gets/sets the audio sample size.
	ACESSOR(Size, int, size)

	// Gets/sets the buffer containing the data.
	ACESSOR(Buffer, const std::vector<byte>&, dataBuffer)

	// Return the proper resource group for this resource.
	GETTER(ResourceGroup, ResourceGroup::Enum, ResourceGroup::Audio)

protected:

	// Sound frequency.
	int frequency;

	// Sound channels.
	int channels;

	// Sound sample size.
	int size;

	// Holds the sound data.
	std::vector<byte> dataBuffer;
};

TYPEDEF_INTRUSIVE_POINTER_FROM_TYPE( Sound );

//-----------------------------------//

} // end namespace