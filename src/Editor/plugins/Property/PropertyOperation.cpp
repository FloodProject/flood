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
	assert( type && type->isClass() );
	assert( field != nullptr );

	const Type& field_type = field->type;
	LogDebug("Changed property value: %s", field->name.c_str() );

	bool isResource = field_type.inherits<Resource>();

	if( field->isPointer() && isResource )
	{
		std::string val = value.As<std::string>();

		ResourceManager* rm = GetEditor().getEngine()->getResourceManager();
		ResourcePtr resource = rm->loadResource(val);

		if( !resource )
			return;

		field->set<ResourcePtr>(object, resource);
	}
	else if( field_type.isEnum() )
	{
		int val = value.As<int>();
		field->set<int>(object, val);
	}
	else if( field_type.isPrimitive() )
	{
		const Primitive& prim_type = (const Primitive&) field_type;

		if( prim_type.isBool() )
		{
			bool val = value.As<bool>();
			field->set<bool>(object, val);
		}
		//-----------------------------------//
		else if( prim_type.isInteger() )
		{
			int val = value.As<int>();
			field->set<int>(object, val);
		}
		//-----------------------------------//
		else if( prim_type.isFloat() )
		{
			float val = value.As<float>();
			field->set<float>(object, val);
		}
		//-----------------------------------//
		else if( prim_type.isString() )
		{
			std::string val = value.As<std::string>();
			field->set<std::string>(object, val);
		}
		//-----------------------------------//
		else if( prim_type.isColor() )
		{
			Color val = value.As<Color>();
			field->set<Color>(object, val);
		}
		//-----------------------------------//
		else if( prim_type.isVector3() )
		{
			Vector3 val = value.As<Vector3>();
			field->set<Vector3>(object, val);
		}
		//-----------------------------------//
		else if( prim_type.isQuaternion() )
		{
			Quaternion val = value.As<Quaternion>();
			field->set<Quaternion>(object, val);
		}
		//-----------------------------------//
		else if( prim_type.isBitfield() )
		{
			int val = value.As<int>();
			field->set<int>(object, val);
		}
		//-----------------------------------//
		else assert( false );
	}
	else assert( false );
}

//-----------------------------------//

} } // end namespaces