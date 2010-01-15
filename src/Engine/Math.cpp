/************************************************************************
*
* vaporEngine (2008-2010)
*
************************************************************************/

#pragma once

#include "vapor/PCH.h"
#include "vapor/math/Math.h"

namespace vapor { namespace math {

//-----------------------------------//

float logn( float base, float value )
{
	return log( value ) / log( base );
}

//-----------------------------------//

float random( float min, float max )
{
	return ((max - min) 
		* ( (float(rand()) / RAND_MAX ) ) 
		+ min);
}

//-----------------------------------//

float degreeToRadian( float degree )
{
	return degree * PI / 180.0f;
}

//-----------------------------------//

} } // end namespaces