/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "vapor/core/Type.h"

namespace vapor {

//-----------------------------------//

/**
 * Represents a field in a class.
 */

class VAPOR_API Field
{
public:

	Field( const Type& type );

	template<typename T>
	const T& get( const void* object ) const
	{
		T* ptr = (T*)((byte*)object + offset);
		return *ptr;
	}

	template<typename T>
	void set( const void* object, const T& value ) const
	{
		T* ptr = (T*)((byte*)object + offset);
		*ptr = value;
	}

public:

	short offset;
	short size;
	std::string name;
	const Type& type;
};

//-----------------------------------//

} // end namespace
