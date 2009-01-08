/************************************************************************
*
* vaporEngine by triton (2008)
*
*	<http://www.portugal-a-programar.org>
*
* DO WHAT THE FUCK YOU WANT TO PUBLIC LICENSE
* Version 2, December 2004
* 
* DO WHAT THE FUCK YOU WANT TO PUBLIC LICENSE
* TERMS AND CONDITIONS FOR COPYING, DISTRIBUTION AND MODIFICATION
* 
* 0. You just DO WHAT THE FUCK YOU WANT TO.
*
************************************************************************/

#pragma once

/**
 * Represents a TTF (TrueType) font file. The class loads the file using
 * the cross-platform FreeType library, parses all the font glyphs, and
 * packs them all into a big texture (texture atlas).
 */

#include "vapor/resources/Font.h"

namespace vapor {
	namespace resources {

class TTF : public Font
{

public:
	TTF(const char* font);
	~TTF();

private:

};

} } // end namespaces

