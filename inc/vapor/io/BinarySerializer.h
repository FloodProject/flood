/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Core/API.h"
#include "Serialization.h"

namespace vapor {

//-----------------------------------//
	
class CORE_API BinarySerializer : public Serializer
{
	DECLARE_UNCOPYABLE(BinarySerializer)

	BinarySerializer();

//protected:
//
//	// Serializes the class fields.
//	void serializeFields( const Class& type, void* object, Json::Value& );
//
//	// Deserializes the class fields.
//	void deserializeFields( const Class& type, void* object, const Json::Value& );
//
//	// Converts an enum type to a value.
//	Json::Value valueFromEnum( const Field& field, void* object );
//
//	// Converts a primitive type to a value.
//	Json::Value valueFromPrimitive( const Field& field, void* object );
//
//	// Sets the field in the object to the given value.
//	void setFieldFromValue( const Field& field, void* object, const Json::Value& value );
};

//-----------------------------------//

} // end namespace