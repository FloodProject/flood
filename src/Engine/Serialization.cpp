/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "vapor/PCH.h"
#include "vapor/Serialization.h"
#include "vapor/vfs/File.h"
#include <clocale>

namespace vapor {

//-----------------------------------//

Json::Value toJson( const math::Vector3& vec )
{
	Json::Value v;
	v[0u] = vec.x;
	v[1u] = vec.y;
	v[2u] = vec.z;
	return v;
}

//-----------------------------------//

Json::Value toJson( const math::EulerAngles& ang )
{
	Json::Value v;
	v[0u] = ang.x;
	v[1u] = ang.y;
	v[2u] = ang.z;
	return v;
}

//-----------------------------------//

Json::Value toJson( const math::Color& c )
{
	Json::Value v;
	v[0u] = c.r;
	v[1u] = c.g;
	v[2u] = c.b;
	v[3u] = c.a;
	return v;
}

//-----------------------------------//

void serializeToFile( Json::Value& root, const std::string& name )
{
	// Always use the platform independent "C" locale when writing JSON,
	// json-cpp assumes this and will write wrong data with another locale.
	LocaleSaveRestore c;

	NativeFile file( name, AccessMode::Write );

	if( !file.open() )
		return;

	file.write( root.toStyledString() );
}

//-----------------------------------//

} // end namespace 