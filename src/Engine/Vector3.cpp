/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "vapor/PCH.h"
#include "vapor/math/Vector2.h"
#include "vapor/math/Vector3.h"
#include "vapor/math/Vector4.h"

namespace vapor {

//-----------------------------------//

const VAPOR_API Vector2i Vector2i::Zero( 0, 0 );
const VAPOR_API Vector2i Vector2i::UnitX( 1, 0 );
const VAPOR_API Vector2i Vector2i::UnitY( 0, 1 );

//-----------------------------------//

const VAPOR_API Vector3 Vector3::Zero( 0.0f, 0.0f, 0.0f );
const VAPOR_API Vector3 Vector3::UnitX( 1.0f, 0.0f, 0.0f );
const VAPOR_API Vector3 Vector3::UnitY( 0.0f, 1.0f, 0.0f );
const VAPOR_API Vector3 Vector3::UnitZ( 0.0f, 0.0f, 1.0f );

//-----------------------------------//

const VAPOR_API Vector4 Vector4::Zero( 0.0f, 0.0f, 0.0f, 0.0f );
const VAPOR_API Vector4 Vector4::UnitX( 1.0f, 0.0f, 0.0f, 0.0f );
const VAPOR_API Vector4 Vector4::UnitY( 0.0f, 1.0f, 0.0f, 0.0f );
const VAPOR_API Vector4 Vector4::UnitZ( 0.0f, 0.0f, 1.0f, 0.0f );
const VAPOR_API Vector4 Vector4::UnitW( 0.0f, 0.0f, 0.0f, 1.0f );

//-----------------------------------//

} // end namespace