/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Core/API.h"
#include "Object.h"

namespace vapor {

//-----------------------------------//

Object::Object()
{ }

//-----------------------------------//

void Object::serialize( const Serializer& serializer )
{

}

//-----------------------------------//

Object* Object::createInstance() const
{
	return nullptr;
}

//-----------------------------------//

Object* Object::clone() const
{
	return nullptr;
}

//-----------------------------------//

} // end namespace