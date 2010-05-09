/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include <json/json.h>
#include "vapor/math/Vector3.h"
#include "vapor/math/EulerAngles.h"
#include "vapor/math/Color.h"

namespace Json { class Value; }

namespace vapor {

//-----------------------------------//

#define DECLARE_SERIALIZABLE()		\
	virtual void serialize( Json::Value& )

/**
 * Classes that want to be serializable should inherit this class and
 * implement the virtual method. The members will be written to disk
 * in JSON format, a lightweight readable alternative to XML.
 */

class VAPOR_API Serializable
{
public:

	virtual void serialize( Json::Value& ) = 0;
};

//-----------------------------------//
// Serialization Helpers             // 
//-----------------------------------//

void serializeToFile( Json::Value& root, const std::string& name );
Json::Value toJson( const math::Vector3& vec );
Json::Value toJson( const math::EulerAngles& ang );
Json::Value toJson( const math::Color& c );

//-----------------------------------//

} // end namespace 