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

typedef std::map<std::string, int> EnumValuesMap;
typedef std::pair<const std::string, int> EnumValuesPair;

//-----------------------------------//

/**
 * Represents an enum in the type system.
 */

class VAPOR_API Enum : public Type
{
public:

	Enum(const std::string& name);

	// Adds a new enumeration to this enum.
	void addValue( const std::string str, int value );

	// Gets the name of this enum value.
	const std::string& getString(int value) const;

	// Gets the values of this enum.
	const EnumValuesMap& getValues() const;

protected:

	EnumValuesMap values;
};

#define DECLARE_ENUM() static vapor::Enum& getType();

#define BEGIN_ENUM(enumName)							\
	vapor::Enum& enumName::getType() {					\
	static vapor::Enum enuHmm(TOSTRING(enumName));		\
	
#define ENUM(enumName)									\
	enuHmm.addValue(TOSTRING(enumName), (int)enumName);

#define END_ENUM() return enuHmm; }

//-----------------------------------//

} // end namespace