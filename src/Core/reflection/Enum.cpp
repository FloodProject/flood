/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Core/API.h"
#include "core/Enum.h"
#include "Log.h"

namespace vapor {

//-----------------------------------//

Enum::Enum(const String& name, int size)
	: Type(MetaType::Enumeration, name, size)
{ }

//-----------------------------------//

void Enum::addValue( const String& name, int value )
{
	EnumValuesMap::iterator it = values.find(name);
	
	if( it == values.end() )
		values[name] = value;
}

//-----------------------------------//

const String& Enum::getName(int value) const
{
	EnumValuesMap::iterator it;
	
	for( it = values.begin(); it != values.end(); it++ )
	{
		if( it->second == value )
			return it->first;
	}

	static String empty;
	return empty;
}

//-----------------------------------//

const int Enum::getValue(const String& name) const
{
	if(values.find(name) == values.end())
		return -1;

	return values[name];
}

//-----------------------------------//

const EnumValuesMap& Enum::getValues() const
{
	return values;
}

//-----------------------------------//

} // end namespace