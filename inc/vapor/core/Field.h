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

	template<typename T> const T& get(void* object) const
	{
		T* ptr = (T*)((bool*)object + offset);
		return *ptr;
	}

	template<typename T> void set( const T& value ) const
	{
		(*(T*)ptr) = value;
	}

public:

	short offset;
	short size;
	std::string name;
	const Type& type;
};

//-----------------------------------//

} // end namespace
