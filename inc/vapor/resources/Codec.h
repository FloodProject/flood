/************************************************************************
*
* vaporEngine by triton © (2008-2009)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#pragma once

#include "vapor/Engine.h"
#include "vapor/vfs/File.h"
#include "vapor/resources/Image.h"

using namespace vapor::vfs;

namespace vapor {
	namespace resources {

/**
 * Codec interface for classes that want to provide image
 * coding and decoding services for a given image file format.
 */

class Codec
{

public:

	/**
	 * Decodes a given image file.
	 */
	virtual Image& decode(File& image) = 0;

	/**
	 * Gets a list of recognized extensions by this codec.
	 */
	list<string>& getExtensions() { return _extensions; };

	/**
	 * Gets the name of this codec.
	 */
	virtual string getName() = 0;

protected:
	
	// holds all file extensions recognized by this codec
	list<string> _extensions;
};

} } // end namespaces