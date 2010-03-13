/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "vapor/PCH.h"

#include "vapor/resources/Resource.h"

namespace vapor { namespace resources {

//-----------------------------------//

namespace ResourceGroup
{
	const std::string getString(Enum e) 
	{
		switch(e) 
		{
		case General:
			return "General";
		case Images:
			return "Images";
		case Meshes:
			return "Meshes";
		case Fonts:
			return "Fonts";
		case Shaders:
			return "Shaders";
		case Audio:
			return "Audio";
		case Scripts:
			return "Scripts";
		default:
			return "(unknown)";
		}
	}
}

//-----------------------------------//

Resource::Resource( const std::string& uri )
	: uri( uri )
{

}

//-----------------------------------//

Resource::~Resource()
{

}

//-----------------------------------//

bool Resource::reload( )
{
	return true;
}

//-----------------------------------//

} } // end namespaces