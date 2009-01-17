/************************************************************************
*
* vaporEngine by triton © (2008-2009)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#pragma once

#include "vapor/resources/Codec.h"

namespace vapor {
	namespace resources {

/**
 * This codec provides PNG decoding services using the lightweight, 
 * thin, tiny, awesome, picoPNG library. :)
 */

class PicoPNG : public Codec
{

public:

	PicoPNG();

	virtual Image& decode(File& image);

	virtual string getName() { return "picoPNG"; }
};

} } // end namespaces
