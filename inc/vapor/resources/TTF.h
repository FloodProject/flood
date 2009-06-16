/************************************************************************
*
* vaporEngine (2008-2009)
*
*	<http://www.portugal-a-programar.org>
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

