/************************************************************************
*
* vaporEngine by triton © (2008-2009)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#pragma once

#include "vapor/Platform.h"
#include "vapor/resources/Resource.h"
#include "vapor/resources/ResourceManager.h"
#include <al.h>

namespace vapor {
	namespace resources {

/**
 * Possible pixel formats for an image.
 */

namespace SoundFormat 
{
	enum Enum 
	{
    MONO8 = AL_FORMAT_MONO8,
    MONO16 = AL_FORMAT_MONO16,
    STEREO8 = AL_FORMAT_STEREO8,
    STEREO16 = AL_FORMAT_STEREO16
	};

	string getString(Enum e);
}

/**
 * Represents a sound in the engine. A sound holds uncompressed audio
 * data that can be played back.
 *
 */

class Sound : public Resource
{
public:

	Sound(const SoundFormat::Enum format, const int frequency, std::vector< char > const& data );

	virtual ~Sound();
	
	// Gets the frequency of the sound.
	const int getFrequency() const { return frequency; }

	// Returns the format of the sound
	SoundFormat::Enum getFormat() const { return format; }

	// Gets a pointer to the buffer containing the data.
	std::vector< char > const& getBuffer() const { return dataBuffer; };

	void setBuffer(std::vector< char > const& data) { dataBuffer = data; }

	// Return the proper Images resource group
	virtual ResourceGroup::Enum getResourceGroup() { return ResourceGroup::Audio; }

private:

	// sound frequency
	int frequency;

	// sound format
	SoundFormat::Enum format;

	// holds the image data
	std::vector< char > dataBuffer;
};

} } // end namespaces