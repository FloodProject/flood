/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "vapor/PCH.h"
#include "vapor/resources/Resource.h"

namespace vapor {

//-----------------------------------//

BEGIN_ENUM(ResourceGroup)
	ENUM(General)
	ENUM(Images)
	ENUM(Meshes)
	ENUM(Fonts)
	ENUM(Shaders)
	ENUM(Audio)
	ENUM(Scripts)
END_ENUM()

BEGIN_CLASS(Resource)
	FIELD_PRIMITIVE(Resource, string, path)
END_CLASS()

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

std::string Resource::getBasePath() const
{
	assert( !path.empty() );

	size_t n = path.find_last_of(".");
	assert( n != std::string::npos );

	return path.substr(0, n);
}

//-----------------------------------//

} // end namespace