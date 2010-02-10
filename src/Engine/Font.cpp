/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "vapor/PCH.h"

#include "vapor/resources/Font.h"

namespace vapor { namespace resources {

//-----------------------------------//

Font::Font( const std::string& name, int size )
	: name( name ), size( size )
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
