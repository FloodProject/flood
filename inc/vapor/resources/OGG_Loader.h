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

namespace vapor {
	namespace resources {

/**
 * Loader for OGG vorbis files.
 */

class OGG_Loader : public ResourceLoader
{
public:

	OGG_Loader();

	// Decode an image file to a buffer
	virtual Sound* decode(File& file);

	// Gets the name of this codec.
	virtual string getName() { return "OGG"; }

	// Gets the list of extensions this codec can handle.
	virtual list<string>& getExtensions() { return extensions; }

	// Overrides this to return the right resource group.
	virtual ResourceGroup::Enum getResourceGroup() { return ResourceGroup::Audio; }

protected:

	int getALFormat(SoundFormat::Enum format);

	// holds all file extensions recognized by this codec
	list<string> extensions;
};

} } // end namespaces

#endif