/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Core/API.h"
#include "Math/Vector.h"

NAMESPACE_CORE_BEGIN

//-----------------------------------//

const API_CORE Vector2 Vector2::Zero(  0, 0 );
const API_CORE Vector2 Vector2::UnitX( 1, 0 );
const API_CORE Vector2 Vector2::UnitY( 0, 1 );

//-----------------------------------//

const API_CORE Vector3 Vector3::Zero(  0, 0, 0 );
const API_CORE Vector3 Vector3::UnitX( 1, 0, 0 );
const API_CORE Vector3 Vector3::UnitY( 0, 1, 0 );
const API_CORE Vector3 Vector3::UnitZ( 0, 0, 1 );

//-----------------------------------//

const API_CORE Vector4 Vector4::Zero(  0, 0, 0, 0 );
const API_CORE Vector4 Vector4::UnitX( 1, 0, 0, 0 );
const API_CORE Vector4 Vector4::UnitY( 0, 1, 0, 0 );
const API_CORE Vector4 Vector4::UnitZ( 0, 0, 1, 0 );
const API_CORE Vector4 Vector4::UnitW( 0, 0, 0, 1 );

//-----------------------------------//

NAMESPACE_CORE_END