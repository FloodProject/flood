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

Resource::Resource()
	: status( ResourceStatus::Unloaded )
{ }

//-----------------------------------//

bool Resource::reload()
{
	return true;
}

//-----------------------------------//

bool Resource::isLoaded() const
{
	return getStatus() == ResourceStatus::Loaded;
}

//-----------------------------------//

std::string Resource::getBaseURI() const
{
	assert( !uri.empty() );

	size_t n = uri.find_last_of(".");
	assert( n != std::string::npos );

	return uri.substr( 0, n);
}

//-----------------------------------//

} } // end namespaces