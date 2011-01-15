/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "Reflection.h"
#include "Serialization.h"

namespace vapor {

//-----------------------------------//

/**
 * Objects are the root class for types using the reflection services.
 * You can test hierarchy status aswell as get the reflection classes
 * that provide more related information about the types.
 */

class CORE_API Object
{
public:

	Object();

	// Gets the type of the object.
	virtual const Class& getType() const = 0;

	// Serializes the object to a stream.
	virtual void serialize( const Serializer& serializer );

	// Returns a new instance of this object.
	virtual Object* createInstance() const;

	// Clones an object.
	virtual Object* clone() const;
};

//-----------------------------------//

} // end namespace
