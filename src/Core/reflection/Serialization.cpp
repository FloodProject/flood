/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "vapor/PCH.h"
#include "vapor/Serialization.h"
#include "vapor/Utilities.h"
#include "vapor/scene/Group.h"
#include "vapor/math/Color.h"
#include "vapor/math/EulerAngles.h"
#include "vapor/vfs/File.h"
#include "vapor/Headers.h"

namespace vapor {

//-----------------------------------//

Serializer::Serializer()
{ }

//-----------------------------------//

bool Serializer::openFromFile( const std::string& name )
{
	LocaleSwitch c;

	FileStream file( name, StreamMode::Read );

	if( !file.open() )
		return false;

	std::string text;
	file.read(text);

	Json::Reader jsonReader;
	bool success = jsonReader.parse(text, rootValue, false);
	
	return success;
}

//-----------------------------------//

void Serializer::saveToFile( const std::string& name )
{
	// Always switch to the platform independent "C" locale when writing
	// JSON, else the library will format the data erroneously.
	LocaleSwitch c;

	FileStream file( name, StreamMode::Write );

	if( !file.open() )
		return;

	file.write( rootValue.toStyledString() );
}

//-----------------------------------//

void Serializer::serializeScene(const ScenePtr& scene)
{
	rootValue.clear();

	const Class& sceneType = scene->getInstanceType();
	Json::Value& sceneValue = rootValue[sceneType.getName()];
	
	serializeFields( sceneType, scene.get(), sceneValue );
		
	const std::vector<EntityPtr>& entities = scene->getEntities();
	for( uint i = 0; i < entities.size(); i++ )
	{
		const EntityPtr& entity = entities[i];
		const Class& type = entity->getInstanceType();

		if( type.inherits<Group>() )
			assert( false );

		sceneValue["nodes"].append(serializeEntity(entity));
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

		const EntityPtr& node = deserializeEntity(nodeValue);
		scene->add(node);
	}

	return scene;
}

//-----------------------------------//

Json::Value Serializer::serializeEntity(const EntityPtr& node)
{
	Json::Value value;

	const Class& nodeType = node->getInstanceType();
	void* object = node.get();

	// Serialize node fields.
	serializeFields( nodeType, object, value );
    
    // Serialize components.
	const ComponentMap& components = node->getComponents();
	
	ComponentMap::const_iterator it;
	for( it = components.cbegin(); it != components.cend(); it++ )
	{
		const Class& type = *(it->first);
		const ComponentPtr& component = it->second;

		Json::Value& componentValue = value["components"][type.getName()];

		serializeFields( type, component.get(), componentValue );
	}

	return value;
}

//-----------------------------------//

EntityPtr Serializer::deserializeEntity( const Json::Value& nodeValue )
{
	TypeRegistry& typeRegistry = TypeRegistry::getInstance();

	EntityPtr entity( new Entity() );
	const Class& type = entity->getInstanceType();

	deserializeFields(type, entity.get(), nodeValue);

    // Components.
	const Json::Value& values = nodeValue["components"];

	Json::Value::Members members = values.getMemberNames();

	for( uint i = 0; i < members.size(); i++ )
	{
		const std::string& name = members[i];
		const Class* type = (Class*) typeRegistry.getType(name);

		if( !type )
		{
			Log::warn("Type '%s' was not found in registry", name.c_str() );
			continue;
		}

		const Json::Value& value = values[name];

		ComponentPtr component( (Component*) type->createInstance() );
		deserializeFields(*type, component.get(), value);

		entity->addComponent(component);
	}

	return entity;
}

//-----------------------------------//

void Serializer::serializeFields(const Class& type, void* object, Json::Value& value)
{
	if( type.getParent() )
	{
		const Class& parent = (Class&) *type.getParent();
		serializeFields(parent, object, value);
	}

	const FieldsMap& fields = type.getFields();
	
	FieldsMap::const_iterator it;
	for( it = fields.cbegin(); it != fields.cend(); it++ )
	{
		const Field& field = *(it->second);
		const Type& field_type = field.type;

		void* realObject = object;

		if( field.isPointer() )
		{
			// Get the real object from the pointer.
			const ResourcePtr ptr = field.get<ResourcePtr>(object);
			realObject = (void*) ptr.get();
		}

		if( !realObject )
			continue;

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

void Serializer::deserializeFields(const Class& type, void* object, const Json::Value& fieldsValue)
{
	if( fieldsValue.isNull() )
		return;

	const Json::Value::Members& members = fieldsValue.getMemberNames();

	for( uint i = 0; i < members.size(); i++ )
	{
		const std::string& name = members[i];
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

static Vector3 convertValueToVector3( const Json::Value& value )
{
	Vector3 vec(
		float(value[0u].asDouble()),
		float(value[1u].asDouble()),
		float(value[2u].asDouble()) );

	return vec;
}

//-----------------------------------//

static Color convertValueToColor( const Json::Value& value )
{
	Color color(
		float(value[0u].asDouble()),
		float(value[1u].asDouble()),
		float(value[2u].asDouble()),
		float(value[3u].asDouble()) );

	return color;
}

//-----------------------------------//

static Quaternion convertValueToQuaternion( const Json::Value& value )
{
	if(value.size() == 3)
	{
		EulerAngles ang(
			float(value[0u].asDouble()),
			float(value[1u].asDouble()),
			float(value[2u].asDouble()));

		return Quaternion(ang);
	}

	Quaternion quat(
		float(value[0u].asDouble()),
		float(value[1u].asDouble()),
		float(value[2u].asDouble()),
		float(value[3u].asDouble()) );

	return quat;
}

//-----------------------------------//

void Serializer::setFieldFromValue( const Field& field, void* object, const Json::Value& value )
{
	if( !field.type.isPrimitive() )
	{
		Log::debug( "field: %s", field.name.c_str() );
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
	//-----------------------------------//
	else if( type.isQuaternion() )
	{
		Quaternion val = convertValueToQuaternion(value);
		field.set<Quaternion>(object, val);
	}
	//-----------------------------------//
	else if( type.isBitfield() )
	{
		std::string str = value.asString();
		std::bitset<32> val(str);
		field.set< std::bitset<32> >(object, val);
	}
	else
	//-----------------------------------//
		assert( false );
}

//-----------------------------------//

static Json::Value convertVector3( const Vector3& vec )
{
	Json::Value v;
	
	v.append(vec.x);
	v.append(vec.y);
	v.append(vec.z);
	
	return v;
}

//-----------------------------------//

static Json::Value convertColor( const Color& c )
{
	Json::Value v;
	
	v.append(c.r);
	v.append(c.g);
	v.append(c.b);
	v.append(c.a);
	
	return v;
}

//-----------------------------------//

static Json::Value convertQuaternion( const Quaternion& q )
{
	Json::Value v;
	
	v.append(q.x);
	v.append(q.y);
	v.append(q.z);
	v.append(q.w);
	
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
	//-----------------------------------//
	else if( type.isQuaternion() )
	{
		Quaternion quat = field.get<Quaternion>(object);
		v = convertQuaternion(quat);
	}
	//-----------------------------------//
	else if( type.isBitfield() )
	{
		std::bitset<32> bits = field.get< std::bitset<32> >(object);
		v = bits.to_string();
	}
	else
	//-----------------------------------//
		assert( false );

	return v;
}

//-----------------------------------//

} // end namespace