/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
//// 
//// fuckEngine by triton
////
//// License: fuckGNU License
////
//// Changelog:
////	(18/08/08) Initial implementantion
////
//// Thanks to:
////	Zero (love ya bitch)
////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

#pragma once

//#include "Engine.h"
#include "Vector3.h"

namespace vapor {
	namespace math {


class Ray 
{
protected:
	Vector3 mOrigin;
	Vector3  mDirection;

public:
	Ray() 
	{
	mOrigin->zero();
	mDirection->Vector3(0, 0, 1);
	}
	Ray(Vector3& origin, Vector3& direction): mOrigin(origin),mDirection( direction) {}

	/** Sets the origin of the ray. */
    void setOrigin(const Vector3& origin) {mOrigin = origin;} 
    /** Gets the origin of the ray. */
    const Vector3& getOrigin(void) const {return mOrigin;} 

    /** Sets the direction of the ray. */
    void setDirection(const Vector3& dir) {mDirection = dir;} 
    /** Gets the direction of the ray. */
    const Vector3& getDirection(void) const {return mDirection;} 

	

};


} } // end namespaces