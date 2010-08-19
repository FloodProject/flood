/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "vapor/PCH.h"
#include "vapor/core/Enum.h"

namespace vapor {

//-----------------------------------//

Enum::Enum(const std::string& name)
	: Type(MetaType::Enumeration, name)
{ }

//-----------------------------------//

void Enum::addValue( const std::string str, int value )
{
	EnumValuesMap::iterator it = values.find(str);
	
	if( it == values.end() )
		values[str] = value;
}

//-----------------------------------//

const std::string& Enum::getString(int value) const
{
	foreach( const EnumValuesPair& p, values )
	{
		if( p.second == value )
			return p.first;
	}

	assert( false );
}

//-----------------------------------//

const EnumValuesMap& Enum::getValues() const
{
	return values;
}

//-----------------------------------//

} // end namespace