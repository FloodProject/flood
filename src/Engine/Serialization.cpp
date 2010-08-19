/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "vapor/PCH.h"
#include "vapor/Serialization.h"
#include "vapor/scene/Group.h"
#include "vapor/math/Color.h"
#include "vapor/math/EulerAngles.h"
#include "vapor/vfs/File.h"
#include "vapor/Headers.h"

namespace vapor {

//-----------------------------------//

void Serializer::serializeScene(const ScenePtr& scene)
{
	rootValue.clear();

	const Class& sceneType = scene->getInstanceType();
	Json::Value& sceneValue = rootValue[sceneType.getName()];
	
	serializeFields( sceneType, scene.get(), sceneValue );
		
	foreach( const NodePtr& node, scene->getChildren() )
	{
		const Class& nodeType = node->getInstanceType();

		if( nodeType.inherits<Group>() )
			assert( false );

		sceneValue["nodes"].append(serializeNode(node));
	}
}

//-----------------------------------//

ScenePtr Serializer::deserializeScene()
{
	ScenePtr scene( new Scene() );

	const Class& sceneType = scene->getInstanceType();
	const Json::Value& sceneValue = rootValue[sceneType.getName()];

	deserializeFields(sceneType, scene.get(), sceneValue);

	const Json::Value& nodesValue = sceneValue["nodes"];

	for( uint i = 0; i < nodesValue.size(); i++ )
	{
		const Json::Value& nodeValue = nodesValue[i];

		if( nodeValue.isNull() || nodeValue.empty() )
			continue;

		const NodePtr& node = deserializeNode(nodeValue);
		scene->add(node);
	}

	return scene;
}

//-----------------------------------//

Json::Value Serializer::serializeNode(const NodePtr& node)
{
	Json::Value value;

	const Class& nodeType = node->getInstanceType();
	void* object = node.get();

	// Serialize node fields.
	serializeFields( nodeType, object, value );
    
    // Serialize components.
	foreach( const ComponentMapPair& p, node->getComponents() )
	{
		const Class& type = *(p.first);
		const ComponentPtr& component = p.second;

		Json::Value& componentValue = value["components"][type.getName()];

		serializeFields( type, component.get(), componentValue );
	}

	return value;
}

//-----------------------------------//

ComponentPtr createComponent(const Type* type)
{
	ComponentPtr c;

	if( type->is<Transform>() )
		c.reset( new Transform() );
	else if( type->is<Light>() )
		c.reset( new Light() );
	else if( type->is<Model>() )
		c.reset( new Model() );
	else if( type->is<Skydome>() )
		c.reset( new Skydome() );
	else if( type->is<Terrain>() )
		c.reset( new Terrain() );
	else
		assert( false );

	return c;
}

//-----------------------------------//

NodePtr Serializer::deserializeNode( const Json::Value& nodeValue )
{
	NodePtr node( new Node() );

	const Class& nodeType = node->getInstanceType();

	deserializeFields(nodeType, node.get(), nodeValue);

    // Components.
	const Json::Value& componentValues = nodeValue["components"];

	foreach( const std::string& name, componentValues.getMemberNames() )
	{
		TypeRegistry& typeRegistry = TypeRegistry::getInstance();
		const Class* type = (Class*) typeRegistry.getType(name);

		if( !type )
			continue;

		ComponentPtr component = createComponent(type);
		assert( component != nullptr );

		const Json::Value& componentValue = componentValues[name];
		deserializeFields(*type, component.get(), componentValue);

		node->addComponent(component);
	}

	return node;
}

//-----------------------------------//

void Serializer::deserializeFields(const Class& type, void* object, const Json::Value& fieldsValue)
{
	foreach( const std::string& name, fieldsValue.getMemberNames() )
	{
		const Field* field = type.getField(name);

		if( !field )
			continue;

		if( field->isPointer() )
		{
			const Json::Value& fieldValue = fieldsValue[name];
			const Json::Value::Members& members = fieldValue.getMemberNames();
			assert( members.size() > 0 );

			const Json::Value& resValue = fieldValue[members.front()];
			const std::string& resPath = resValue["path"].asString();
			
			Engine* engine = Engine::getInstancePtr();
			ResourceManager* rm = engine->getResourceManager();
			ResourcePtr res = rm->loadResource(resPath);

			field->set<ResourcePtr>(object, res);
		}
		else
			setFieldFromValue( *field, object, fieldsValue[name] );
	}
}

//-----------------------------------//

void Serializer::serializeFields(const Class& type, void* object, Json::Value& value)
{
	if( type.getParent() )
	{
		const Class& parent = (Class&) *type.getParent();
		serializeFields(parent, object, value);
	}

	foreach( const FieldsPair& p, type.getFields() )
	{
		const Field& field = *p.second;
		const Type& field_type = field.type;

		void* realObject = object;

		if( field.isPointer() )
		{
			// Get the real object from the pointer.
			const boost::intrusive_ptr<Resource> ptr =
				field.get<boost::intrusive_ptr<Resource>>(object);
			
			realObject = (void*) ptr.get();
		} 

		if( field_type.isClass() || field_type.isStruct() )
		{
			Json::Value& classValue = value[field.name][field_type.getName()];
			serializeFields((Class&) field_type, realObject, classValue);
		}
		else if( field_type.isPrimitive() )
		{
			value[field.name] = valueFromPrimitive(field, realObject);
		}
		else if( field_type.isEnum() )
		{
			value[field.name] = valueFromEnum(field, realObject);
		}
		else
			assert( false );
	}
}

//-----------------------------------//

Vector3 convertValueToVector3( const Json::Value& value )
{
	Vector3 vec(
		float(value[0u].asDouble()),
		float(value[1u].asDouble()),
		float(value[2u].asDouble()) );

	return vec;
}

//-----------------------------------//

Color convertValueToColor( const Json::Value& value )
{
	Color color(
		float(value[0u].asDouble()),
		float(value[1u].asDouble()),
		float(value[2u].asDouble()),
		float(value[3u].asDouble()) );

	return color;
}

//-----------------------------------//

void Serializer::setFieldFromValue( const Field& field, void* object, const Json::Value& value )
{
	if( !field.type.isPrimitive() )
	{
		debug( "field: %s", field.name.c_str() );
		return;
	}

	//assert( field.type.isPrimitive() );
	const Primitive& type = (const Primitive&) field.type;

	if( type.isBool() )
	{
		bool val = value.asBool();
		field.set<bool>(object, val);
	}
	//-----------------------------------//
	else if( type.isInteger() )
	{
		int val = value.asInt();
		field.set<int>(object, val);
	}
	//-----------------------------------//
	else if( type.isFloat() )
	{
		double val = value.asDouble();
		field.set<float>(object, float(val));
	}
	//-----------------------------------//
	else if( type.isString() )
	{
		std::string val = value.asString();
		field.set<std::string>(object, val);
	}
	//-----------------------------------//
	else if( type.isColor() )
	{
		Color val = convertValueToColor(value);
		field.set<Color>(object, val);
	}
	//-----------------------------------//
	else if( type.isVector3() )
	{
		Vector3 val = convertValueToVector3(value);
		field.set<Vector3>(object, val);
	}
	else
	//-----------------------------------//
	{
		assert( false );
	}
}

//-----------------------------------//

bool Serializer::openFromFile( const std::string& name )
{
	LocaleSaveRestore c;

	NativeFile file( name, AccessMode::Read );

	if( !file.open() )
		return false;

	std::string str = file.readString();

	Json::Reader jsonReader;
	bool success = jsonReader.parse(str, rootValue, false);
	
	return success;
}

//-----------------------------------//

void Serializer::saveToFile( const std::string& name )
{
	// Always switch to the platform independent "C" locale when writing
	// JSON, else the library will format the data erroneously.
	LocaleSaveRestore c;

	NativeFile file( name, AccessMode::Write );

	if( !file.open() )
		return;

	file.write( rootValue.toStyledString() );
}

//-----------------------------------//

Json::Value convertVector3( const Vector3& vec )
{
	Json::Value v;
	
	v.append(vec.x);
	v.append(vec.y);
	v.append(vec.z);
	
	return v;
}

//-----------------------------------//

Json::Value convertEulerAngles( const EulerAngles& ang )
{
	Json::Value v;
	
	v.append(ang.x);
	v.append(ang.y);
	v.append(ang.z);
	
	return v;
}

//-----------------------------------//

Json::Value convertColor( const Color& c )
{
	Json::Value v;
	
	v.append(c.r);
	v.append(c.g);
	v.append(c.b);
	v.append(c.a);
	
	return v;
}

//-----------------------------------//

Json::Value Serializer::valueFromEnum( const Field& field, void* object )
{
	assert( field.type.isEnum() );
	const Enum& type = (const Enum&) field.type;

	Json::Value v;

	//foreach( const EnumValuesPair& p, type.getValues() )
	//{
	//	arrValues.Add( p.first, p.second );
	//}

	return v;
}

//-----------------------------------//

Json::Value Serializer::valueFromPrimitive( const Field& field, void* object )
{
	assert( field.type.isPrimitive() );
	const Primitive& type = (const Primitive&) field.type;

	Json::Value v;

	if( type.isBool() )
	{
		bool val = field.get<bool>(object);
		v = val;
	}
	//-----------------------------------//
	else if( type.isInteger() )
	{
		int val = field.get<int>(object);
		v = val;
	}
	//-----------------------------------//
	else if( type.isFloat() )
	{
		float val = field.get<float>(object);
		v = val;
	}
	//-----------------------------------//
	else if( type.isString() )
	{
		std::string val = field.get<std::string>(object);
		v = val;
	}
	//-----------------------------------//
	else if( type.isColor() )
	{
		Color val = field.get<Color>(object);
		v = convertColor(val);
	}
	//-----------------------------------//
	else if( type.isVector3() )
	{
		Vector3 vec = field.get<Vector3>(object);
		v = convertVector3(vec);
	}
	else
	//-----------------------------------//
	{
		assert( false );
	}

	return v;
}

//-----------------------------------//

} // end namespace 