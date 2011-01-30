/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Core/API.h"
#include "Serialization.h"
#include "Reflection.h"
#include "Log.h"

namespace vapor {

//-----------------------------------//

ObjectSerializer::ObjectSerializer( Serializer& serializer )
	: serializer(serializer)
{ }

//-----------------------------------//

void ObjectSerializer::save( const ObjectData& object )
{
	assert( object.type != nullptr );
	assert( object.instance != nullptr );

	serializeType(object);
	serializer.flushStream();
}

//-----------------------------------//

void ObjectSerializer::serializeType(ObjectData object)
{
	const Type& type = *object.type;

	if( type.isClass() || type.isStruct() )
	{
		serializeClass(object);		
	}
	else if( type.isPrimitive() )
	{
		serializePrimitive(object);
	}
	else if( type.isEnum() )
	{
		//value[field.name] = valueFromEnum(field, realObjectData);
	}
	else assert( false );
}

//-----------------------------------//

void ObjectSerializer::serializeClass(ObjectData object)
{
	Class& type = (Class&) *object.type;
	serializer.beginClass(type);

	if( type.parent )
	{
		ObjectData parent;
		parent.instance = object.instance;
		parent.type = (Type*) type.parent;
		
		serializeClass(parent);
	}

	FieldsMap::const_iterator it;
	const FieldsMap& fields = type.getFields();

	for( it = fields.begin(); it != fields.end(); it++ )
	{
		const Field& field = *(it->second);
		serializeField(object, field);
	}

	serializer.endClass();
}

//-----------------------------------//

void ObjectSerializer::serializeField(ObjectData object, const Field& field)
{
	if( !object.instance ) return;
	
	const Type& type = field.type;

	object.instance = (byte*) object.instance + field.offset;
	object.type = (Type*) &type;

	if( field.qualifiers & Qualifier::Array )
	{
		std::vector<byte>& array = * (std::vector<byte>*) object.instance;

		serializer.beginArray(field.type);

		// TODO: 64-bits support.

		uint numElems = array.size() / field.size;
		for(uint i = 0; i < numElems; i++)
		{
			ObjectData elem;
			elem.instance = (&array.front() + i * type.size);
			elem.type = (Type*) &field.type;

			serializeType(elem);
		}

		serializer.endArray();
	}
	else if( field.isPointer() )
	{
		// Get the real object from the pointer.
		//const ResourcePtr ptr = field.get<ResourcePtr>(object);
		//realObjectData = (void*) ptr.get();	
	}
	else 
	{
		serializePrimitive(object);
	}
}

//-----------------------------------//

void ObjectSerializer::serializePrimitive(const ObjectData& object)
{
	const Primitive& type = (const Primitive&) *object.type;

	if( type.isBool() )
	{
		bool* val = (bool*) object.instance;
		serializer.writeBool(type, *val);
	}
	//-----------------------------------//
	else if( type.isInteger() )
	{
		int* val = (int*) object.instance;
		serializer.writeInt(type, *val);
	}
	//-----------------------------------//
	else if( type.isFloat() )
	{
		float* val = (float*) object.instance;
		serializer.writeFloat(type, *val);
	}
	//-----------------------------------//
	else if( type.isString() )
	{
		std::string* val = (std::string*) object.instance;
		serializer.writeString(type, *val);
	}
	//-----------------------------------//
	else if( type.isColor() )
	{
		Color* val = (Color*) object.instance;
		serializer.writeColor(type, *val);
	}
	//-----------------------------------//
	else if( type.isVector3() )
	{
		Vector3* vec = (Vector3*) object.instance;
		serializer.writeVector3(type, *vec);
	}
	//-----------------------------------//
	else if( type.isQuaternion() )
	{
		Quaternion* quat = (Quaternion*) object.instance;
		serializer.writeQuaternion(type, *quat);
	}
	//-----------------------------------//
	else if( type.isBitfield() )
	{
		std::bitset<32>* bits = (std::bitset<32>*) object.instance;
		serializer.writeBitfield(type, *bits);
	}
	else assert( false );
}

//-----------------------------------//

//bool Serializer::openFromFile( const std::string& name )
//{
//	LocaleSwitch c;
//
//	FileStream file( name, StreamMode::Read );
//
//	if( !file.open() )
//		return false;
//
//	std::string text;
//	file.read(text);
//
//	Json::Reader jsonReader;
//	bool success = jsonReader.parse(text, rootValue, false);
//	
//	return success;
//}
//
////-----------------------------------//
//
//void Serializer::saveToFile( const std::string& name )
//{
//	// Always switch to the platform independent "C" locale when writing
//	// JSON, else the library will format the data erroneously.
//	LocaleSwitch c;
//
//	FileStream file( name, StreamMode::Write );
//
//	if( !file.open() )
//		return;
//
//	file.write( rootValue.toStyledString() );
//}
//
////-----------------------------------//
//
//void Serializer::serializeScene(const ScenePtr& scene)
//{
//	rootValue.clear();
//
//	const Class& sceneType = scene->getType();
//	Json::Value& sceneValue = rootValue[sceneType.getName()];
//	
//	serializeFields( sceneType, scene.get(), sceneValue );
//		
//	const std::vector<EntityPtr>& entities = scene->getEntities();
//	for( uint i = 0; i < entities.size(); i++ )
//	{
//		const EntityPtr& entity = entities[i];
//		const Class& type = entity->getType();
//
//		if( type.inherits<Group>() )
//			assert( false );
//
//		sceneValue["nodes"].append(serializeEntity(entity));
//	}
//}
//
////-----------------------------------//
//
//ScenePtr Serializer::deserializeScene()
//{
//	ScenePtr scene( new Scene() );
//
//	const Class& sceneType = scene->getType();
//	const Json::Value& sceneValue = rootValue[sceneType.getName()];
//
//	deserializeFields(sceneType, scene.get(), sceneValue);
//
//	const Json::Value& nodesValue = sceneValue["nodes"];
//
//	for( uint i = 0; i < nodesValue.size(); i++ )
//	{
//		const Json::Value& nodeValue = nodesValue[i];
//
//		if( nodeValue.isNull() || nodeValue.empty() )
//			continue;
//
//		const EntityPtr& node = deserializeEntity(nodeValue);
//		scene->add(node);
//	}
//
//	return scene;
//}
//
////-----------------------------------//
//
//Json::Value Serializer::serializeEntity(const EntityPtr& node)
//{
//	Json::Value value;
//
//	const Class& nodeType = node->getType();
//	void* object = node.get();
//
//	// Serialize node fields.
//	serializeFields( nodeType, object, value );
//    
//    // Serialize components.
//	const ComponentMap& components = node->getComponents();
//	
//	ComponentMap::const_iterator it;
//	for( it = components.cbegin(); it != components.cend(); it++ )
//	{
//		const Class& type = *(it->first);
//		const ComponentPtr& component = it->second;
//
//		Json::Value& componentValue = value["components"][type.getName()];
//
//		serializeFields( type, component.get(), componentValue );
//	}
//
//	return value;
//}
//
////-----------------------------------//
//
//EntityPtr Serializer::deserializeEntity( const Json::Value& nodeValue )
//{
//	Registry& typeRegistry = Type::GetRegistry();
//
//	EntityPtr entity( new Entity() );
//	const Class& type = entity->getType();
//
//	deserializeFields(type, entity.get(), nodeValue);
//
//    // Components.
//	const Json::Value& values = nodeValue["components"];
//
//	Json::Value::Members members = values.getMemberNames();
//
//	for( uint i = 0; i < members.size(); i++ )
//	{
//		const std::string& name = members[i];
//		const Class* type = (Class*) typeRegistry.getType(name);
//
//		if( !type )
//		{
//			Log::warn("Type '%s' was not found in registry", name.c_str() );
//			continue;
//		}
//
//		const Json::Value& value = values[name];
//
//		ComponentPtr component( (Component*) type->createInstance() );
//		deserializeFields(*type, component.get(), value);
//
//		entity->addComponent(component);
//	}
//
//	return entity;
//}
//
////-----------------------------------//
//
//void Serializer::deserializeFields(const Class& type, void* object, const Json::Value& fieldsValue)
//{
//	if( fieldsValue.isNull() )
//		return;
//
//	const Json::Value::Members& members = fieldsValue.getMemberNames();
//
//	for( uint i = 0; i < members.size(); i++ )
//	{
//		const std::string& name = members[i];
//		const Field* field = type.getField(name);
//
//		if( !field )
//			continue;
//
//		if( field->isPointer() )
//		{
//			const Json::Value& fieldValue = fieldsValue[name];
//			const Json::Value::Members& members = fieldValue.getMemberNames();
//			assert( members.size() > 0 );
//
//			const Json::Value& resValue = fieldValue[members.front()];
//			const std::string& resPath = resValue["path"].asString();
//			
//			Engine* engine = GetEngine();
//			ResourceManager* rm = engine->getResourceManager();
//			ResourcePtr res = rm->loadResource(resPath);
//
//			field->set<ResourcePtr>(object, res);
//		}
//		else
//			setFieldFromValue( *field, object, fieldsValue[name] );
//	}
//}

//-----------------------------------//

} // end namespace