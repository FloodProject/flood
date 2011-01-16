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
#include "Utilities.h"

namespace vapor { namespace editor {

//-----------------------------------//

struct TagName
{
	long id;
	const char* name;
};

TagName TagNames[] = 
{
	{ Tags::NonPickable,		"NonPickable" },
	{ Tags::NonTransformable,	"NonTransformable" },
	{ Tags::NonCollidable,		"NonCollidable" },
	{ Tags::NonCulled,			"NonCulled" },
	{ Tags::UpdateTransformsOnly,	"UpdateTransformsOnly" },
	{ EditorTags::EditorOnly,		"EditorOnly" },
};

//-----------------------------------//

static ResourcePtr askResource()
{
	wxFileDialog fd( &GetEditor(), wxFileSelectorPromptStr,
			wxEmptyString, wxEmptyString, "Resource files | *.*",
			wxFD_DEFAULT_STYLE | wxFD_FILE_MUST_EXIST );

	if( fd.ShowModal() != wxID_OK )
		return nullptr;

	std::string path( fd.GetPath() );
	path = Path::normalize(path);
	
	ResourceManager* rm = GetEditor().getEngine()->getResourceManager();
	return rm->loadResource( Path::getFile(path) );
}

//-----------------------------------//

class ResourceProperty : public wxLongStringProperty
{
public:

	ResourceProperty(const std::string& name)
		: wxLongStringProperty(name, wxPG_LABEL)
	{}

	virtual bool OnButtonClick( wxPropertyGrid* propGrid, wxString& value )
	{
		ResourcePtr resource = askResource();

		if( !resource )
			return false;

		//const Type& resourceType = resource->getType();

		//PropertyData* data = (PropertyData*) GetClientObject();

		//if( &resourceType != data->type )
		//	return false;

		SetValueInEvent( Path::getFile( resource->getPath() ) );

		return true;
	}
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

static wxAny getComposedPropertyValue(wxPGProperty* prop, const Type& type)
{
	if( type.isPrimitive() )
	{
		const Primitive& prim = (const Primitive&) type;

		bool isVector = prim.isVector3();
		bool isQuaternion = prim.isQuaternion();
		
		if( isVector || isQuaternion )
		{
			wxPGProperty* X = prop->GetPropertyByName("X");
			wxPGProperty* Y = prop->GetPropertyByName("Y");
			wxPGProperty* Z = prop->GetPropertyByName("Z");

			wxAny x = X->GetValue();
			wxAny y = Y->GetValue();
			wxAny z = Z->GetValue();

			Vector3 vec( x.As<float>(), y.As<float>(), z.As<float>() );
			
			if(isVector)
				return wxAny(vec);
			else
				return wxAny( Quaternion((EulerAngles&) vec) );
		}
	}

	return prop->GetValue();
}

//-----------------------------------//

PropertyPage::PropertyPage( wxWindow* parent, wxWindowID id, 
	const wxPoint& pos, const wxSize& size )
	: wxPropertyGrid(parent, id, pos, size,
	wxPG_DEFAULT_STYLE | wxPG_SPLITTER_AUTO_CENTER)
{
	// Events bindings.
	Bind(wxEVT_PG_CHANGED, &PropertyPage::onPropertyChanged, this);
	Bind(wxEVT_PG_CHANGING, &PropertyPage::onPropertyChanging, this);
	Bind(wxEVT_IDLE, &PropertyPage::onIdle, this);

	// Make the default font a little smaller.
	SetFont( GetEditor().GetFont().Scaled(0.9f) );

	// Switch to slighty lighter colors.
	wxColour color = wxLIGHT_GREY->ChangeLightness(120);
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
	op->description = "Property changed";
	op->grid = this;
	op->type = (Class*) data->type;
	op->field = (Field*) data->field;
	op->object = (void*) data->object;
	op->oldValue = propertyValue;
	op->newValue = value;
	
	UndoManager* undoManager = GetEditor().getUndoManager();
	undoManager->registerOperation(op);

	op->redo();
}

//-----------------------------------//

void PropertyPage::onIdle(wxIdleEvent& event)
{
	if( !updateMemoryWatches() )
		return;

	//Log::debug("Memory watches detected changes");

	const EntityPtr& entity = selectedEntity.lock();
	showEntityProperties(entity);
}

//-----------------------------------//

void PropertyPage::showEntityProperties( const EntityPtr& node )
{
	selectedEntity = node;

	memoryWatches.clear();
	updateMemoryWatches();

	Clear();

    // Entity properties
	appendObjectFields( node->getType(), node.get() );

	// Transform should be the first component to be displayed.
	TransformPtr transform = node->getTransform();

	if( transform )
		appendObjectFields( Transform::getStaticType(), transform.get() );
    
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
	
	const std::vector<Field*>& fields = type.getFieldsVector();
	
	for( uint i = 0; i < fields.size(); i++ )
	{
		const Field& field = *fields[i];

		bool isCompound = field.type.isClass() || field.type.isStruct();
		bool isResource = field.type.inherits<Resource>();

		if( isCompound && !isResource )
		{
			void* addr = (byte*) object + field.offset;
			appendObjectFields((Class&) field.type, addr, false);
			continue;
		}

		wxPGProperty* prop = createProperty(type, field, object);

		if(!prop)
			continue;

		wxString name = convertToReadable( field.name );
		prop->SetLabel( name );

		//while( prop->GetChildCount() )
		//{
		//	wxPGProperty* child = prop->Item(0);
		//	prop->RemoveChild(child);
		//}

		Append(prop);

		while( !composed.empty() )
		{
			wxPGProperty* cp = composed.back();
			composed.pop_back();
			AppendIn( prop, cp );
		}

		Collapse(prop);
	}
}

//-----------------------------------//

wxPGProperty* PropertyPage::createProperty(const Class& type, const Field& field, void* object)
{
	wxPGProperty* prop = nullptr;

	bool isResource = field.type.inherits<Resource>();

	if( field.isPointer() && isResource )
		prop = createResourceProperty(field, object);
	else if( field.type.isPrimitive() )
		prop = createPrimitiveProperty(field, object);
	else if( field.type.isEnum() )
		prop = createEnumProperty(field, object);

	PropertyData* data = new PropertyData();
	data->type = &type;
	data->field = &field;
	data->object = object;

	prop->SetClientObject(data);

	return prop;
}

//-----------------------------------//

wxPGProperty* PropertyPage::createResourceProperty(const Field& field, void* object)
{
	ResourcePtr res = field.get<ResourcePtr>(object);
			
	std::string file;
			
	if( res )
		file = Path::getFile( res->getPath() );

	const std::string& name = field.type.getName();

	wxPGProperty* prop = new ResourceProperty(name);
	prop->SetValueFromString(file);

	return prop;
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

	return prop;
}

//-----------------------------------//

wxFloatProperty* PropertyPage::createFloatProperty(const char* name, float value)
{
	wxFloatProperty* prop = new wxFloatProperty( name, wxPG_LABEL, value );
	prop->SetAttribute(wxPG_FLOAT_PRECISION, 3);
	return prop;
}

//-----------------------------------//

wxPGProperty* PropertyPage::createPrimitiveProperty(const Field& field, void* object)
{
	wxPGProperty* prop = nullptr;
	const Primitive& type = (const Primitive&) field.type;

	if( type.isBool() )
	{
		bool val = field.get<bool>(object);
		prop = new wxBoolProperty( wxEmptyString, wxPG_LABEL, val );
	}
	//-----------------------------------//
	else if( type.isInteger() )
	{
		int val = field.get<int>(object);
		prop = new wxIntProperty( wxEmptyString, wxPG_LABEL, val );
	}
	//-----------------------------------//
	else if( type.isFloat() )
	{
		float val = field.get<float>(object);
		prop = createFloatProperty( (const char*) wxEmptyString, val );
	}
	//-----------------------------------//
	else if( type.isString() )
	{
		std::string val = field.get<std::string>(object);
		prop = new wxStringProperty( wxEmptyString, wxPG_LABEL, val );
	}
	//-----------------------------------//
	else if( type.isColor() )
	{
		Color val = field.get<Color>(object);
		wxColour wx( val.r*255, val.g*255, val.b*255, val.a*255 );
		prop = new wxColourProperty( wxEmptyString, wxPG_LABEL, wx );
	}
	//-----------------------------------//
	else if( type.isVector3() )
	{
		Vector3 vec = field.get<Vector3>(object);
		prop = new wxStringProperty( wxEmptyString, wxPG_LABEL, "<composed>" );
		composed.push_back( createFloatProperty("X", vec.x) );
		composed.push_back( createFloatProperty("Y", vec.y) );
		composed.push_back( createFloatProperty("Z", vec.z) );
		//prop->AppendChild( createFloatProperty("X", vec.x) );
		//prop->AppendChild( createFloatProperty("Y", vec.y) );
		//prop->AppendChild( createFloatProperty("Z", vec.z) );
	}
	//-----------------------------------//
	else if( type.isQuaternion() )
	{
		Quaternion quat = field.get<Quaternion>(object);
		EulerAngles ang = quat.getEulerAngles(quat);
		prop = new wxStringProperty( wxEmptyString, wxPG_LABEL, "<composed>" );
		composed.push_back( createFloatProperty("X", ang.x) );
		composed.push_back( createFloatProperty("Y", ang.y) );
		composed.push_back( createFloatProperty("Z", ang.z) );
		//prop->AppendChild( createFloatProperty("X", ang.x) );
		//prop->AppendChild( createFloatProperty("Y", ang.y) );
		//prop->AppendChild( createFloatProperty("Z", ang.z) );
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
	}
	//-----------------------------------//
	else
	{
		Log::debug( "Unknown property type: '%s'", type.getName().c_str() );
	}

	return prop;
}

//-----------------------------------//

} } // end namespaces