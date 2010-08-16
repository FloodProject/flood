/************************************************************************
*
* vapor3D Editor © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "PCH.h"
#include "PropertyOperation.h"

namespace vapor { namespace editor {

//-----------------------------------//

Color getColorFromWx( wxColour& colour )
{
	return Color(
		colour.Red()   / 255.0f,
		colour.Green() / 255.0f,
		colour.Blue()  / 255.0f,
		colour.Alpha() / 255.0f );
}

//-----------------------------------//

void PropertyOperation::undo()
{ 
	setFieldValue(prevValue);
}

//-----------------------------------//

void PropertyOperation::redo()
{
	setFieldValue(newValue);
}

//-----------------------------------//

void PropertyOperation::setFieldValue(const wxAny& value)
{
	assert( type && type->isClass() );
	assert( field != nullptr );

	const Type& field_type = field->type;
	debug("Changed property value: %s", field->name.c_str() );

	if( field_type.isEnum() )
	{
		int val = value.As<int>();
		type->setFieldValue<int>(field->name, object, val);
	}
	else if( field_type.isPrimitive() )
	{
		const Primitive& prim_type = (const Primitive&) field_type;

		if( prim_type.isBool() )
		{
			bool val = value.As<bool>();
			type->setFieldValue<bool>(field->name, object, val);
		}

		else if( prim_type.isInteger() )
		{
			int val = value.As<int>();
			type->setFieldValue<int>(field->name, object, val);
		}

		else if( prim_type.isFloat() )
		{
			float val = value.As<float>();
			type->setFieldValue<float>(field->name, object, val);
		}

		else if( prim_type.isString() )
		{
			wxString val = value.As<wxString>();
			type->setFieldValue<std::string>(field->name, object, (std::string) val);
		}

		else if( prim_type.isColor() )
		{
			wxColour val = value.As<wxColour>();
			type->setFieldValue<Color>(field->name, object, getColorFromWx(val));
		}

		else if( prim_type.isVector3() )
		{
			Vector3 val = value.As<Vector3>();
			type->setFieldValue<Vector3>(field->name, object, val );
		}
	}
	else
		assert( false );
}

//-----------------------------------//

} } // end namespaces