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
	//: translation(nullptr), orientation(nullptr)
{
	
}

//-----------------------------------//

Transformable::~Transformable()
{
	//delete translation;
}

//-----------------------------------//

void Transformable::translate( const math::Vector3& tr )
{
	//if(!translation) 
	//{
	//	translation = shared_ptr<Vector3>(new Vector3(tr));
	//	return;
	//} 

	//translation += tr;
	translation += tr;
}

//-----------------------------------//

void Transformable::rotate( float xang, float yang, float zang )
{

}

//-----------------------------------//

} } // end namespaces