/************************************************************************
*
* vapor3D Editor © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Editor/API.h"
#include "PropertyPage.h"
#include "PropertyOperation.h"
#include "Editor.h"
#include "EditorIcons.h"
#include "EditorTags.h"
#include "UndoManager.h"
#include "Core/Utilities.h"
#include "Document.h"

#ifdef ENABLE_PLUGIN_PROPERTY

namespace vapor { namespace editor {

//-----------------------------------//

static wxString convertToReadable(wxString str)
{
	if( str.IsEmpty() ) return wxEmptyString;

	str.Replace("_", " ");

	wxString format;
	format.Append(wxUniChar(toupper(str[0])));

	// Add spaces between words.
	for( size_t i = 1; i < str.Len(); i++ )
	{
		format.Append(str[i]);

		if( i < str.Len()-1 )
		{
			if( !isspace(str[i]) && islower(str[i]) && isupper(str[i+1]) )
				format.Append(" ");
		}
	}

	return format;
}

//-----------------------------------//

struct TagName
{
	int32 id;
	const char* name;
};

TagName TagNames[] = 
{
	{ Tags::NonPickable,			"NonPickable" },
	{ Tags::NonTransformable,		"NonTransformable" },
	{ Tags::NonCollidable,			"NonCollidable" },
	{ Tags::NonCulled,				"NonCulled" },
	{ Tags::UpdateTransformsOnly,	"UpdateTransformsOnly" },
	{ EditorTags::EditorOnly,		"EditorOnly" },
};

static wxPGChoices getTagChoices()
{
	wxPGChoices choices;

	for( size_t i = 0; i < ARRAY_SIZE(TagNames); i++ )
	{
		const TagName& tag = TagNames[i];
			
		wxString name( String(tag.name) );
		choices.Add( convertToReadable(name), tag.id );
	}

	return choices;
}

//-----------------------------------//

bool ReflectionIsResourceHandle(const Field* field)
{
	Type* type = field->type;

	bool isClass = ReflectionIsComposite(type);
	bool isResource = isClass && ClassInherits((Class*) type, ReflectionGetType(Resource));
	
	return isResource && FieldIsHandle(field);
}

//-----------------------------------//

static ResourceHandle askResource()
{
	wxFileDialog fd( &GetEditor(), wxFileSelectorPromptStr,
			wxEmptyString, wxEmptyString, "Resource files | *.*",
			wxFD_DEFAULT_STYLE | wxFD_FILE_MUST_EXIST );

	if( fd.ShowModal() != wxID_OK )
		return ResourceHandle(HandleInvalid);

	String path( fd.GetPath() );
	path = PathNormalize(path);
	
	ResourceManager* res = GetResourceManager();
	return res->loadResource( PathGetFile(path) );
}

//-----------------------------------//

class ResourceProperty : public wxLongStringProperty
{
public:

	ResourceProperty(const String& name)
		: wxLongStringProperty(name, wxPG_LABEL)
	{}

	virtual bool OnButtonClick( wxPropertyGrid* propGrid, wxString& value )
	{
		ResourceHandle resource = askResource();

		if( !resource )
			return false;

		//const Type& resourceType = resource->getType();

		//PropertyData* data = (PropertyData*) GetClientObject();

		//if( &resourceType != data->type )
		//	return false;

		SetValueInEvent( PathGetFile( resource.Resolve()->getPath() ) );

		return true;
	}
};

//-----------------------------------//

PropertyPage::PropertyPage( wxWindow* parent )
	: wxPropertyGrid(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxPG_DEFAULT_STYLE | wxPG_SPLITTER_AUTO_CENTER)
	, currentObject(nullptr)
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

void PropertyPage::reset()
{
	currentObject = nullptr;
	memoryWatches.clear();
	Clear();
}

//-----------------------------------//

void PropertyPage::resetObject(const Object* object)
{
	if( currentObject != object )
		reset();
}

//-----------------------------------//

void PropertyPage::onIdle(wxIdleEvent& event)
{
	if( !updateMemoryWatches() )
		return;

	LogDebug("Memory watches detected changes");
}

//-----------------------------------//

void PropertyPage::onPropertyChanging(wxPropertyGridEvent& event)
{
	wxPGProperty* prop = event.GetProperty();
	propertyValue = getPropertyValue(prop);
}

//-----------------------------------//

void PropertyPage::onPropertyChanged(wxPropertyGridEvent& event)
{
	wxPGProperty* prop = event.GetProperty();
	
	if(	!prop->GetClientObject() ) prop = prop->GetParent();
	PropertyData* data = (PropertyData*) prop->GetClientObject();

	PropertyOperation* propOperation = new PropertyOperation();
	propOperation->description = "Property changed";
	propOperation->type = (Class*) data->type;
	propOperation->field = (Field*) data->field;
	propOperation->object = (void*) data->object;
	propOperation->oldValue = propertyValue;
	propOperation->newValue = getPropertyValue(prop);
	propOperation->grid = this;
	
	UndoManager* undoManager = GetEditor().getDocument()->getUndoManager();
	undoManager->registerOperation(propOperation);

	propOperation->redo();
}

//-----------------------------------//

void PropertyPage::showProperties( Object* object, bool resetObject )
{
	if( resetObject ) reset();
	currentObject = object;

	Class* klass = ClassGetType(object);

	appendObjectFields(*klass, object);
}

//-----------------------------------//

void PropertyPage::showEntityProperties( const EntityPtr& entity )
{
	currentObject = entity.get();

    // Entity properties.
	appendObjectFields( *entity->getType(), entity.get() );

	// Transform properties.
	TransformPtr transform = entity->getTransform();

	if( transform )
		appendObjectFields( *ReflectionGetType(Transform), transform.get() );
    
    // Other components properties.
	const ComponentMap& components = entity->getComponents();
	
	ComponentMap::const_iterator it;
	for( it = components.begin(); it != components.end(); it++ )
	{
		Class* type = it->first;
		const ComponentPtr& component = it->second;

		if( ReflectionIsEqual(type, ReflectionGetType(Transform)) )
			continue;

		appendObjectFields( *type, component.get() );
	}
}

//-----------------------------------//

void PropertyPage::appendObjectFields(Class& type, void* object, bool newCategory)
{
	if( newCategory )
	{
		const wxString& typeName = convertToReadable(type.name);
		wxPropertyCategory* category = new wxPropertyCategory(typeName);
		Append(category);
	}

	if( type.parent )
	{
		Class& parent = (Class&) *type.parent;
		appendObjectFields(parent, object, false);
	}
	
	const std::vector<Field*>& fields = type.fields;
	
	for( size_t i = 0; i < fields.size(); i++ )
	{
		const Field& field = *fields[i];

		if( ReflectionIsComposite(field.type) && !ReflectionIsResourceHandle(&field) )
		{
			void* addr = (byte*) object + field.offset;
			appendObjectFields((Class&) *field.type, addr, false);
			continue;
		}

		wxPGProperty* prop = createProperty(type, field, object);
		if(!prop) continue;

		Append(prop);
		Collapse(prop);

		wxAny value = getFieldValue(&field, object);
		setPropertyValue(prop, value);

		updateMemoryWatch(&field, object);

		MemoryWatch& watch = memoryWatches[&field];
		watch.property = prop;
		watch.object = object;
	}
}

//-----------------------------------//

wxPGProperty* PropertyPage::createProperty(Class& type, const Field& field, void* object)
{
	wxPGProperty* prop = nullptr;

	if( ReflectionIsResourceHandle(&field) )
		prop = createResourceProperty(field, object);
	else if( ReflectionIsPrimitive(field.type) )
		prop = createPrimitiveProperty(field, object);
	else if( ReflectionIsEnum(field.type) )
		prop = createEnumProperty(field, object);

	assert( prop != nullptr );

	PropertyData* data = new PropertyData();
	data->type = &type;
	data->field = &field;
	data->object = object;

	prop->SetClientObject(data);

	wxString name = convertToReadable( field.name );
	prop->SetLabel( name );

	if( FieldHasQualifier(&field, FieldQualifier::ReadOnly) ) prop->Enable(false);

	return prop;
}

//-----------------------------------//

wxPGProperty* PropertyPage::createResourceProperty(const Field& field, void* object)
{
	wxPGProperty* prop = new ResourceProperty("");
	return prop;
}

//-----------------------------------//

wxPGProperty* PropertyPage::createEnumProperty(const Field& field, void* object)
{
	Enum* type = (Enum*) field.type;
	const EnumValuesMap& values = type->values;
	
	wxPGChoices choices;
	
	EnumValuesMap::const_iterator it;
	for( it = values.begin(); it != values.end(); it++ )
	{
		wxString name = convertToReadable(it->first);
		choices.Add(name, it->second);
	}

	wxPGProperty* prop = new wxEnumProperty(wxEmptyString, wxPG_LABEL, choices);
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
	Primitive* primitive = (Primitive*) field.type;

	switch(primitive->type)
	{
	case Primitive::Bool:
	{
		prop = new wxBoolProperty( wxEmptyString, wxPG_LABEL );
		break;
	}
	//-----------------------------------//
	case Primitive::Int32:
	{
		prop = new wxIntProperty( wxEmptyString, wxPG_LABEL );
		break;
	}
	//-----------------------------------//
	case Primitive::Float:
	{
		prop = createFloatProperty( (const char*) wxEmptyString, 0 );
		break;
	}
	//-----------------------------------//
	case Primitive::String:
	{
		prop = new wxStringProperty( wxEmptyString, wxPG_LABEL );
		break;
	}
	//-----------------------------------//
	case Primitive::Color:
	{
		prop = new wxColourProperty( wxEmptyString, wxPG_LABEL );
		break;
	}
	//-----------------------------------//
	case Primitive::Vector3:
	{
		prop = new wxStringProperty( wxEmptyString, wxPG_LABEL, "<composed>" );
		prop->AppendChild( createFloatProperty("X", 0) );
		prop->AppendChild( createFloatProperty("Y", 0) );
		prop->AppendChild( createFloatProperty("Z", 0) );
		break;
	}
	//-----------------------------------//
	case Primitive::Quaternion:
	{
		prop = new wxStringProperty( wxEmptyString, wxPG_LABEL, "<composed>" );
		prop->AppendChild( createFloatProperty("X", 0) );
		prop->AppendChild( createFloatProperty("Y", 0) );
		prop->AppendChild( createFloatProperty("Z", 0) );
		break;
	}
	//-----------------------------------//
	case Primitive::Bitfield:
	{
		wxPGChoices choices = getTagChoices();
		prop = new wxFlagsProperty( wxEmptyString, wxPG_LABEL, choices );
		break;
	}
	//-----------------------------------//
	default:
	{
		LogDebug( "Unknown property type: '%s'", primitive->name );
	} }

	return prop;
}

//-----------------------------------//

static wxColour convertColor( const Color& color )
{
	return wxColour(
		color.r * 255.0f,
		color.g * 255.0f,
		color.b * 255.0f,
		color.a * 255.0f );
}

//-----------------------------------//

wxAny PropertyPage::getFieldValue(const Field* field, void* object)
{
	wxAny value;

	if( ReflectionIsResourceHandle(field) )
	{
		ResourceHandle handle = FieldGet<ResourceHandle>(field, object);
		Resource* resource = handle.Resolve();
		
		if(resource)
		{
			String name = resource->getPath();
			value = PathGetFile(name);
		}
		else
		{
			value = String();
		}

		return value;
	}

	switch(field->type->type)
	{
	case Type::Enumeration:
		value = FieldGet<int32>(field, object);
		break;
	case Type::Primitive:
		value = getFieldPrimitiveValue(field, object);
		break;
	default:
		assert(0);
	}

	return value;
}

//-----------------------------------//

wxAny PropertyPage::getFieldPrimitiveValue(const Field* field, void* object)
{
	wxAny value;

	const Primitive& type = (const Primitive&) *field->type;

	switch(type.type)
	{
	case Primitive::Bool:
		value = FieldGet<bool>(field, object);
		break;
	case Primitive::Int32:
		value = FieldGet<int32>(field, object);
		break;
	case Primitive::Float:
		value = FieldGet<float>(field, object);
		break;
	case Primitive::String:
		value = FieldGet<String>(field, object);
		break;
	case Primitive::Color:
		value = FieldGet<Color>(field, object);
		break;
	case Primitive::Vector3:
		value = FieldGet<Vector3>(field, object);
		break;
	case Primitive::Quaternion:
		value = FieldGet<Quaternion>(field, object);
		break;
	case Primitive::Bitfield:
		value = FieldGet<int32>(field, object);
		break;
	default:
		assert(0);
	}

	return value;
}

//-----------------------------------//

void PropertyPage::setFieldValue(const Field* field, void* object, const wxAny& value)
{
	if(memoryWatches.find(field) == memoryWatches.end())
		return;

	MemoryWatch& watch = memoryWatches[field];

	setPropertyValue(watch.property, value);
	updateMemoryWatch(field, object);
}

//-----------------------------------//

void PropertyPage::setPropertyValue(wxPGProperty* prop, const wxAny& value)
{
	if(	!prop->GetClientObject() ) prop = prop->GetParent();
	
	PropertyData* data = (PropertyData*) prop->GetClientObject();
	
	if( ReflectionIsResourceHandle(data->field) )
	{
		String res = value.As<String>();
		prop->SetValue(res);
	}
	else if( ReflectionIsEnum(data->field->type) )
	{
		int32 val = value.As<int32>();
		prop->SetValue(val);
	}
	else
	{
		setPropertyPrimitiveValue(prop, value);
	}
}

//-----------------------------------//

void PropertyPage::setPropertyPrimitiveValue(wxPGProperty* prop, const wxAny& value)
{
	if(	!prop->GetClientObject() ) prop = prop->GetParent();

	PropertyData* data = (PropertyData*) prop->GetClientObject();
	const Primitive& type = (Primitive&) *data->field->type;

	switch(type.type)
	{
	case Primitive::Bool:
	{

		bool val = value.As<bool>();
		prop->SetValue(val);
		break;
	}
	//-----------------------------------//
	case Primitive::Int32:
	{
		int val = value.As<int>();
		prop->SetValue(val);
		break;
	}
	//-----------------------------------//
	case Primitive::Float:
	{
		float val = value.As<float>();
		prop->SetValue(val);
		break;
	}
	//-----------------------------------//
	case Primitive::String:
	{
		String val = value.As<String>();
		prop->SetValue(val);
		break;
	}
	//-----------------------------------//
	case Primitive::Color:
	{
		Color val = value.As<Color>();
		prop->SetValue( wxAny(convertColor(val)) );
		break;
	}
	//-----------------------------------//
	case Primitive::Vector3:
	{
		Vector3 vec = value.As<Vector3>();
		prop->GetPropertyByName("X")->SetValue(vec.x);
		prop->GetPropertyByName("Y")->SetValue(vec.y);
		prop->GetPropertyByName("Z")->SetValue(vec.z);
		break;
	}
	//-----------------------------------//
	case Primitive::Quaternion:
	{
		const Quaternion& quat = value.As<Quaternion>();
		EulerAngles vec = quat.getEulerAngles();
		prop->GetPropertyByName("X")->SetValue(vec.x);
		prop->GetPropertyByName("Y")->SetValue(vec.y);
		prop->GetPropertyByName("Z")->SetValue(vec.z);
		break;
	}
	//-----------------------------------//
	case Primitive::Bitfield:
	{
		int32 bits = value.As<int32>();
		prop->SetValue(bits);
		break;
	}
	default:
		assert(0 && "Unknown primitive type");
		return;
	}
}

//-----------------------------------//

wxAny PropertyPage::getPropertyValue(wxPGProperty* prop)
{
	if(	!prop->GetClientObject() ) prop = prop->GetParent();

	PropertyData* data = (PropertyData*) prop->GetClientObject();
	Type* type = data->field->type;

	if( ReflectionIsResourceHandle(data->field) )
	{
		return (String) prop->GetValue(); 
	}
	else if( ReflectionIsEnum(type) )
	{
		return prop->GetValue().GetAny().As<int32>();
	}
	else if( ReflectionIsPrimitive(type) )
	{
		return getPropertyPrimitiveValue(prop, data);
	}

	return prop->GetValue();
}

//-----------------------------------//

static Color convertColorFromWx( wxColour& colour )
{
	return Color(
		colour.Red()   / 255.0f,
		colour.Green() / 255.0f,
		colour.Blue()  / 255.0f,
		colour.Alpha() / 255.0f );
}

//-----------------------------------//

wxAny PropertyPage::getPropertyPrimitiveValue(wxPGProperty* prop, PropertyData* data)
{
	const Primitive& type = (const Primitive&) *data->field->type;

	switch(type.type)
	{
	case Primitive::Bool:
	{
		bool val = prop->GetValue().GetAny().As<bool>();
		return val;
		break;
	}
	case Primitive::Int32:
	{
		int32 val = prop->GetValue().GetAny().As<int32>();
		return val;
		break;
	}
	case Primitive::Float:
	{
		float val = prop->GetValue().GetAny().As<float>();
		return val;
		break;
	}
	case Primitive::String:
	{
		wxString val = prop->GetValue().GetAny().As<wxString>();
		return String(val.c_str());
		break;
	}
	case Primitive::Color:
	{
		wxColour val = prop->GetValue().GetAny().As<wxColour>();
		return convertColorFromWx(val);
		break;
	}
	case Primitive::Bitfield:
	{
		int32 bits = prop->GetValue().GetAny().As<int32>();
		return bits;
		break;
	}
	case Primitive::Quaternion:
	case Primitive::Vector3:
	{
		wxPGProperty* X = prop->GetPropertyByName("X");
		wxPGProperty* Y = prop->GetPropertyByName("Y");
		wxPGProperty* Z = prop->GetPropertyByName("Z");

		wxAny x = X->GetValue();
		wxAny y = Y->GetValue();
		wxAny z = Z->GetValue();

		Vector3 vec( x.As<float>(), y.As<float>(), z.As<float>() );
			
		if(type.type == Primitive::Vector3)
			return wxAny(vec);
		else
			return wxAny( Quaternion((EulerAngles&) vec) );
		
		break;
	}
	default:
		assert(0);
	}

	return prop->GetValue();
}

//-----------------------------------//

} } // end namespaces

#endif