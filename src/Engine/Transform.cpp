/************************************************************************
*
* vaporEngine (2008-2009)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#include "vapor/scene/Transform.h"

using namespace vapor::math;

namespace vapor {
	namespace scene {

//-----------------------------------//

Transformable::Transformable()
{

}

//-----------------------------------//

void Transformable::translate( const math::Vector3& tr )
{
	if(!translation) 
	{
		shared_ptr<Vector3> vec(new Vector3(tr));
		translation = vec;
		return;
	} 

	*translation += tr;
}

//-----------------------------------//

void Transformable::rotate( float xang, float yang, float zang )
{

}

//-----------------------------------//

} } // end namespaces