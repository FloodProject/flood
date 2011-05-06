/************************************************************************
*
* vapor3D Editor © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Editor/API.h"
#include "PropertyOperation.h"
#include "Editor.h"

#ifdef ENABLE_PLUGIN_PROPERTY

namespace vapor { namespace editor {

//-----------------------------------//

void PropertyOperation::undo()
{ 
	setFieldValue(oldValue);
	grid->setFieldValue(field, object, oldValue);
}

//-----------------------------------//

void PropertyOperation::redo()
{
	setFieldValue(newValue);
	grid->setFieldValue(field, object, newValue);
}

//-----------------------------------//

void PropertyOperation::setFieldValue(const wxAny& value)
{
	assert( type != nullptr );
	assert( field != nullptr );

	LogDebug("Changed property value: %s", field->type->name);

	bool isClass = ReflectionIsComposite(field->type);
	bool isResource = isClass && ClassInherits((Class*) field->type, ReflectionGetType(Resource));

	if( FieldIsPointer(field) && isResource )
	{
		std::string val = value.As<std::string>();

		ResourceManager* rm = GetResourceManager();
		ResourceHandle resource = rm->loadResource(val);

		if( !resource ) return;

		FieldSet<ResourceHandle>(field, object, resource);

		return;
	}

	switch(field->type->type)
	{
	case Type::Enumeration:
	{
		int32 val = value.As<int32>();
		FieldSet<int32>(field, object, val);
		break;
	}
	case Type::Primitive:
	{
		Primitive* primitive = (Primitive*) field->type;

		switch(primitive->type)
		{
		case Primitive::Bool:
		{
			bool val = value.As<bool>();
			FieldSet<bool>(field, object, val);
			break;
		}
		//-----------------------------------//
		case Primitive::Int32:
		{
			int32 val = value.As<int32>();
			FieldSet<int32>(field, object, val);
			break;
		}
		//-----------------------------------//
		case Primitive::Float:
		{
			float val = value.As<float>();
			FieldSet<float>(field, object, val);
			break;
		}
		//-----------------------------------//
		case Primitive::String:
		{
			std::string val = value.As<std::string>();
			FieldSet<std::string>(field, object, val);
			break;
		}
		//-----------------------------------//
		case Primitive::Color:
		{
			Color val = value.As<Color>();
			FieldSet<Color>(field, object, val);
			break;
		}
		//-----------------------------------//
		case Primitive::Vector3:
		{
			Vector3 val = value.As<Vector3>();
			FieldSet<Vector3>(field, object, val);
			break;
		}
		//-----------------------------------//
		case Primitive::Quaternion:
		{
			Quaternion val = value.As<Quaternion>();
			FieldSet<Quaternion>(field, object, val);
			break;
		}
		//-----------------------------------//
		case Primitive::Bitfield:
		{
			int32 val = value.As<int32>();
			FieldSet<int32>(field, object, val);
			break;
		}
		//-----------------------------------//
		default:
			assert( false );
		}
		break;
	}
	default:
		assert( false );
	}
}

//-----------------------------------//

} } // end namespaces

#endif