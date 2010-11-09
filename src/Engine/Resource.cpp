/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "vapor/PCH.h"
#include "vapor/resources/Resource.h"
#include "vapor/Utilities.h"

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

BEGIN_CLASS_ABSTRACT(Resource)
	FIELD_PRIMITIVE(Resource, string, path)
END_CLASS()

//-----------------------------------//

Resource::Resource()
	: status( ResourceStatus::Loaded )
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
	return String::getBaseFromPath(path);
}

//-----------------------------------//

} // end namespace