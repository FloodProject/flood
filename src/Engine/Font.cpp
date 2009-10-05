/************************************************************************
*
* vaporEngine (2008-2009)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#include "vapor/PCH.h"

#include "vapor/resources/Font.h"

namespace vapor {
	namespace resources {

//-----------------------------------//

Font::Font()
{
	
}

//-----------------------------------//

Font::~Font() 
{

}

//-----------------------------------//

ResourceGroup::Enum Font::getResourceGroup() const
{
	return ResourceGroup::Fonts;
}

//-----------------------------------//

} } // end namespaces
