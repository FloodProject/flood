/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "vapor/scene/Scene.h"

#include <json/json.h>
namespace Json { class Value; }

namespace vapor {

//-----------------------------------//

/**
 * Serialization will write a type object to disk in a format that can
 * be readable later when loading. We are using the JSON format, a small
 * and lightweight alternative to XML.
 */

class VAPOR_API Serializer
{
	DECLARE_UNCOPYABLE(Serializer)

public:

	Serializer();

	// Open the JSON representation from a file.
	bool openFromFile( const std::string& name );

	// Saves the JSON representation to a file.
	void saveToFile( const std::string& name );

	// Serializes the scene.
	void serializeScene( const ScenePtr& scene );

	// Deserializes the scene.
	ScenePtr deserializeScene();

protected:

	// Serializes the node.
	Json::Value serializeEntity( const EntityPtr& node );

	// Deserializes the node.
	EntityPtr deserializeEntity( const Json::Value& nodeValue );

	// Serializes the class fields.
	void serializeFields( const Class& type, void* object, Json::Value& );

	// Deserializes the class fields.
	void deserializeFields( const Class& type, void* object, const Json::Value& );

	// Converts an enum type to a JSON value.
	Json::Value valueFromEnum( const Field& field, void* object );

	// Converts a primitive type to a JSON value.
	Json::Value valueFromPrimitive( const Field& field, void* object );

	// Sets the field in the object to the given value.
	void setFieldFromValue( const Field& field, void* object, const Json::Value& value );

	// Root JSON value.
	Json::Value rootValue;
};

//-----------------------------------//

} // end namespace 