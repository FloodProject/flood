/************************************************************************
*
* vaporEngine (2008-2009)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#include "vapor/PCH.h"

#include "vapor/Platform.h"
#include "vapor/resources/TTF.h"

namespace vapor { namespace resources {

//-----------------------------------//

TTF::TTF(const std::string& font)
	: Font( font, 2 )
{

}

//-----------------------------------//

TTF::~TTF()
{
}

//-----------------------------------//

} } // end namespaces
