/************************************************************************
*
* vapor3D Editor © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "PCH.h"
#include "PropertyPage.h"
#include "PropertyOperation.h"
#include "Editor.h"
#include "EditorIcons.h"
#include "UndoManager.h"
#include <wx/propgrid/advprops.h>

namespace vapor { namespace editor {

//-----------------------------------//

wxString convertToReadable(const wxString& str)
{
	if( str.IsEmpty() )
		return wxEmptyString;

	wxString format;
	format.Append(wxUniChar(toupper(str[0])));

	// Add spaces between words.
	for( uint i = 1; i < str.Len(); i++ )
	{
		format.Append(str[i]);

		if( i < str.Len()-1 )
		{
			if( isupper(str[i]) == islower(str[i+1]) )
				format.Append(" ");
		}
	}

	return format;
}

//-----------------------------------//

wxAny getPropertyRealValue(wxPGProperty* prop, const Type& field_type)
{
	if( field_type.isPrimitive() )
	{
		const Primitive& prim_type = (const Primitive&) field_type;

		if( prim_type.isVector3() )
		{
			wxPGProperty* X = prop->GetPropertyByName("X"); 
			wxPGProperty* Y = prop->GetPropertyByName("Y");
			wxPGProperty* Z = prop->GetPropertyByName("Z");

			wxAny x = X->GetValue();
			wxAny y = Y->GetValue();
			wxAny z = Z->GetValue();

			Vector3 val( x.As<float>(), y.As<float>(), z.As<float>() );
			return wxAny(val);
		}
	}

	return prop->GetValue();
}

//-----------------------------------//

PropertyPage::PropertyPage( EditorFrame* editor,
							 wxWindow* parent, wxWindowID id,
							 const wxPoint& pos, const wxSize& size )
	: wxPropertyGrid(parent, id, pos, size,
		wxPG_DEFAULT_STYLE | wxPG_SPLITTER_AUTO_CENTER)
	, editor(editor)
{
	initControl();
}

//-----------------------------------//

void PropertyPage::initControl()
{
	// Events bindings.
	Bind(wxEVT_PG_CHANGED, &PropertyPage::onPropertyChanged, this);
	Bind(wxEVT_PG_CHANGING, &PropertyPage::onPropertyChanging, this);

	// Make the default font a little smaller.
	SetFont( editor->GetFont().Scaled(0.9f) );

	// Switch to slighty lighter colors.
	wxColour color = wxLIGHT_GREY->ChangeLightness(150);
	SetMarginColour( color );
	SetLineColour( color );
	SetCaptionBackgroundColour( color );
	SetCaptionTextColour( *wxBLACK );
}

//-----------------------------------//

void PropertyPage::onPropertyChanging(wxPropertyGridEvent& event)
{
	wxPGProperty* prop = event.GetProperty();
	if( !prop ) return;

	if(	!prop->GetClientObject() )
		prop = prop->GetParent();

	PropertyData* data = (PropertyData*) prop->GetClientObject();
	assert( data != nullptr );

	propertyValue = getPropertyRealValue(prop, data->field->type);
}

//-----------------------------------//

void PropertyPage::onPropertyChanged(wxPropertyGridEvent& event)
{
	wxPGProperty* prop = event.GetProperty();
	if( !prop ) return;

	// If property has no custom data attached, then it must be a children
	// from composed properties, so we really want the parent property.
	
	if(	!prop->GetClientObject() )
		prop = prop->GetParent();

	PropertyData* data = (PropertyData*) prop->GetClientObject();
	assert( data != nullptr );

	wxAny value = getPropertyRealValue(prop, data->field->type);

	PropertyOperation* op = new PropertyOperation();
	op->type = (Class*) data->type;
	op->field = (Field*) data->field;
	op->object = (void*) data->object;
	op->prevValue = propertyValue;
	op->newValue = value;
	
	UndoManager* undoManager = editor->getUndoManager();
	undoManager->registerOperation(op);

	op->redo();
}

//-----------------------------------//

void PropertyPage::showNodeProperties( const NodePtr& node )
{
	Clear();

    // Node properties
	appendObjectFields( node->getInstanceType(), node.get() );

	// Transform should be the first component to the displayed.
	TransformPtr transform = node->getTransform();
	assert( transform != nullptr );

	appendObjectFields( Transform::getType(), transform.get() );
    
    // Components properties
	foreach( const ComponentMapPair& p, node->getComponents() )
	{
		const Class& type = *(p.first);
		const ComponentPtr& component = p.second;

		if( type.is<Transform>() )
			continue;

		appendObjectFields(type, component.get() );
	}
}

//-----------------------------------//

void PropertyPage::appendObjectFields(const Class& type, void* object, bool newCategory)
{
	if( newCategory )
	{
		const std::string& typeName = type.getName();
		wxPropertyCategory* category = new wxPropertyCategory(typeName);
		Append(category);
	}

	foreach( const FieldsPair& p, type.getFields() )
	{
		const Field& field = *p.second;
		const Type& field_type = field.type;

		wxPGProperty* prop = nullptr;

		if( field_type.isClass() || field_type.isStruct() )
		{
			appendObjectFields((Class&) field_type, object, false);
		}
		else if( field_type.isPrimitive() )
		{
			prop = createPrimitiveProperty(field, object);
		}
		else if( field_type.isEnum() )
		{
			prop = createEnumProperty(field, object);
		}
		else
			assert( false );

		if( prop )
		{
			wxString readableName = convertToReadable( field.name );
			prop->SetLabel( readableName );

			PropertyData* customData = new PropertyData;
			customData->type = &type;
			customData->field = &field;
			customData->object = object;

			prop->SetClientObject(customData);
		}
	}
}

//-----------------------------------//

wxPGProperty* PropertyPage::createEnumProperty(const Field& field, void* object)
{
	wxPGProperty* prop = nullptr;

	assert( field.type.isEnum() );
	const Enum& type = (const Enum&) field.type;

	wxPGChoices arrValues;

	foreach( const EnumValuesPair& p, type.getValues() )
	{
		arrValues.Add( p.first, p.second );
	}

	prop = new wxEnumProperty( wxEmptyString, wxPG_LABEL, arrValues );
	Append( prop );

	return prop;
}

//-----------------------------------//

wxPGProperty* PropertyPage::createPrimitiveProperty(const Field& field, void* object)
{
	wxPGProperty* prop = nullptr;

	assert( field.type.isPrimitive() );
	const Primitive& type = (const Primitive&) field.type;

	if( type.isBool() )
	{
		bool val = field.get<bool>(object);
		prop = new wxBoolProperty( wxEmptyString, wxPG_LABEL, val );
		Append( prop );
	}

	else if( type.isInteger() )
	{
		int val = field.get<int>(object);
		prop = new wxIntProperty( wxEmptyString, wxPG_LABEL, val );
		Append( prop );
	}

	else if( type.isFloat() )
	{
		float val = field.get<float>(object);
		prop = new wxFloatProperty( wxEmptyString, wxPG_LABEL, val );
		Append( prop );
	}

	else if( type.isString() )
	{
		std::string val = field.get<std::string>(object);
		prop = new wxStringProperty( wxEmptyString, wxPG_LABEL, val );
		Append( prop );
	}

	else if( type.isColor() )
	{
		Color val = field.get<Color>(object);
		wxColour wx( val.r*255, val.g*255, val.b*255, val.a*255 );
		prop = new wxColourProperty( wxEmptyString, wxPG_LABEL, wx );
		Append( prop );
	}

	else if( type.isVector3() )
	{
		Vector3 vec = field.get<Vector3>(object);
		
		prop = new wxStringProperty( wxEmptyString, wxPG_LABEL, "<composed>" );
		Append( prop );

		AppendIn( prop, new wxFloatProperty( "X", wxPG_LABEL, vec.x ) );
		AppendIn( prop, new wxFloatProperty( "Y", wxPG_LABEL, vec.y ) );
		AppendIn( prop, new wxFloatProperty( "Z", wxPG_LABEL, vec.z ) );

		Collapse( prop );
	}

	if( !prop )
	{
		debug( "Unknown property type: '%s'", type.getName().c_str() );
		assert( false );
	}

	return prop;
}

//-----------------------------------//

} } // end namespaces