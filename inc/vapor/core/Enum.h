/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "core/Type.h"

namespace vapor {

//-----------------------------------//

typedef std::map<String, int> EnumValuesMap;
typedef std::pair<const String, int> EnumValuesPair;

/**
 * Represents an enum in the type system.
 */

class CORE_API Enum : public Type
{
public:

	Enum(const String& name, int size);

	// Adds a new enumeration to this enum.
	void addValue( const String& name, int value );

	// Gets the name of this enum value.
	const String& getName(int value) const;

	// Gets the value of this enum name.
	const int getValue(const String& name) const;

	// Gets the values of this enum.
	const EnumValuesMap& getValues() const;

protected:

	mutable EnumValuesMap values;
};

//-----------------------------------//

} // end namespace