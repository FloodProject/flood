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

struct TagName
{
	long id;
	const char* name;
};

TagName TagNames[] = 
{
	{ 1 << 24, "NonPickable" },
	{ 1 << 25, "NonTransformable" },
	{ 1 << 26, "NonCollidable" },
	{ 1 << 27, "UpdateTransformsOnly" },
	{ 1 << 31, "EditorOnly" },
};

//-----------------------------------//

static wxString convertToReadable(const wxString& str)
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

static wxAny getComposedPropertyValue(wxPGProperty* prop, const Type& typeField)
{
	if( typeField.isPrimitive() )
	{
		const Primitive& typePrim = (const Primitive&) typeField;

		if( typePrim.isVector3() )
		{
			wxPGProperty* X = prop->GetPropertyByName("X"); 
			wxPGProperty* Y = prop->GetPropertyByName("Y");
			wxPGProperty* Z = prop->GetPropertyByName("Z");

			wxAny x = X->GetValue();
			wxAny y = Y->GetValue();
			wxAny z = Z->GetValue();

			Vector3 vec( x.As<float>(), y.As<float>(), z.As<float>() );
			return wxAny(vec);
		}
		else if( typePrim.isQuaternion() )
		{
			wxPGProperty* X = prop->GetPropertyByName("X"); 
			wxPGProperty* Y = prop->GetPropertyByName("Y");
			wxPGProperty* Z = prop->GetPropertyByName("Z");

			wxAny x = X->GetValue();
			wxAny y = Y->GetValue();
			wxAny z = Z->GetValue();

			EulerAngles ang( x.As<float>(), y.As<float>(), z.As<float>() );
			return wxAny( Quaternion(ang) );
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
	Bind(wxEVT_IDLE, &PropertyPage::onIdle, this);

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

	propertyValue = getComposedPropertyValue(prop, data->field->type);
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

	wxAny value = getComposedPropertyValue(prop, data->field->type);

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

void PropertyPage::onIdle(wxIdleEvent& event)
{
	const EntityPtr& node = selectedEntity.lock();
	
	//if( node )
		//showEntityProperties(node);
}

//-----------------------------------//

void PropertyPage::showEntityProperties( const EntityPtr& node )
{
	selectedEntity = node;

	Clear();

    // Entity properties
	appendObjectFields( node->getInstanceType(), node.get() );

	// Transform should be the first component to the displayed.
	TransformPtr transform = node->getTransform();

	if( transform )
		appendObjectFields( Transform::getType(), transform.get() );
    
    // Components properties
	const ComponentMap& components = node->getComponents();
	
	ComponentMap::const_iterator it;
	for( it = components.cbegin(); it != components.cend(); it++ )
	{
		const Class& type = *(it->first);
		const ComponentPtr& component = it->second;

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

	if( type.getParent() )
	{
		const Class& parent = (Class&) *type.getParent();
		appendObjectFields(parent, object, false);
	}
	
	const FieldsMap& fields = type.getFields();
	
	FieldsMap::const_iterator it;
	for( it = fields.cbegin(); it != fields.cend(); it++ )
	{
		const Field& field = *(it->second);
		const Type& fieldType = field.type;

		wxPGProperty* prop = nullptr;

		if( field.isPointer() )
			continue;

		if( fieldType.isClass() || fieldType.isStruct() )
		{
			void* addr = (byte*) object + field.offset;
			appendObjectFields((Class&) fieldType, addr, false);
		}
		else if( fieldType.isPrimitive() )
		{
			prop = createPrimitiveProperty(field, object);
		}
		else if( fieldType.isEnum() )
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
	
	const EnumValuesMap& values = type.getValues();
	
	EnumValuesMap::const_iterator it;
	for( it = values.cbegin(); it != values.cend(); it++ )
	{
		arrValues.Add( it->first, it->second );
	}

	int val = field.get<int>(object);

	prop = new wxEnumProperty( wxEmptyString, wxPG_LABEL, arrValues, val );
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
	//-----------------------------------//
	else if( type.isInteger() )
	{
		int val = field.get<int>(object);
		prop = new wxIntProperty( wxEmptyString, wxPG_LABEL, val );
		Append( prop );
	}
	//-----------------------------------//
	else if( type.isFloat() )
	{
		float val = field.get<float>(object);
		prop = new wxFloatProperty( wxEmptyString, wxPG_LABEL, val );
		Append( prop );
	}
	//-----------------------------------//
	else if( type.isString() )
	{
		std::string val = field.get<std::string>(object);
		prop = new wxStringProperty( wxEmptyString, wxPG_LABEL, val );
		Append( prop );
	}
	//-----------------------------------//
	else if( type.isColor() )
	{
		Color val = field.get<Color>(object);
		wxColour wx( val.r*255, val.g*255, val.b*255, val.a*255 );
		prop = new wxColourProperty( wxEmptyString, wxPG_LABEL, wx );
		Append( prop );
	}
	//-----------------------------------//
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
	//-----------------------------------//
	else if( type.isQuaternion() )
	{
		Quaternion quat = field.get<Quaternion>(object);
		EulerAngles ang = quat.getEulerAngles(quat);
		
		prop = new wxStringProperty( wxEmptyString, wxPG_LABEL, "<composed>" );
		Append( prop );

		AppendIn( prop, new wxFloatProperty( "X", wxPG_LABEL, ang.x ) );
		AppendIn( prop, new wxFloatProperty( "Y", wxPG_LABEL, ang.y ) );
		AppendIn( prop, new wxFloatProperty( "Z", wxPG_LABEL, ang.z ) );

		Collapse( prop );
	}
	//-----------------------------------//
	else if( type.isBitfield() )
	{
		std::bitset<32> bitfield = field.get<std::bitset<32>>(object);
		ulong bits = bitfield.to_ulong();
		
		wxPGChoices choices;

		for( uint i = 0; i < VAPOR_ARRAY_SIZE(TagNames); i++ )
		{
			const TagName& tag = TagNames[i];
			
			wxString name( std::string(tag.name) );
			choices.Add( convertToReadable(name), tag.id );
		}

		prop = new wxFlagsProperty( wxEmptyString, wxPG_LABEL, choices, bits );
		Append( prop );
	}
	//-----------------------------------//
	else
	{
		Log::debug( "Unknown property type: '%s'", type.getName().c_str() );
		assert( false );
	}

	assert( prop != nullptr );
	return prop;
}

//-----------------------------------//

} } // end namespaces