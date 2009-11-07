/************************************************************************
*
* vaporEngine (2008-2009)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#pragma once

#include "vapor/resources/Font.h"

namespace vapor {
	namespace resources {

//-----------------------------------//

/**
 * Represents a TTF (TrueType) font resource. Loads a TTF font using the
 * cross-platform FreeType library. Parses all the font glyphs of a size,
 * and packs them all into a big texture (texture atlas) for rendering.
 */

class TTF : public Font
{
public:

	TTF(const std::string& font);
	~TTF();

private:

};

//-----------------------------------//

} } // end namespaces

