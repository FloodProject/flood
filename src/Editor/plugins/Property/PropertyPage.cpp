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
#include "Core/PluginManager.h"
#include "Core/Utilities.h"
#include "Document.h"
#include "../Resources/ResourcesPlugin.h"
#include "../Resources/ResourcesBrowser.h"

#ifdef ENABLE_PLUGIN_PROPERTY

NAMESPACE_EDITOR_BEGIN

//-----------------------------------//

static wxString GetReadableName(wxString str)
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

bool ReflectionIsResourceHandle(const Field* field)
{
	Type* type = field->type;

	bool isClass = ReflectionIsComposite(type);
	bool isResource = isClass && ClassInherits((Class*) type, ReflectionGetType(Resource));
	
	return isResource && FieldIsHandle(field);
}

//-----------------------------------//

static ResourceHandle GetResourceUsingBrowser( ResourceGroup::Enum group )
{
	ResourcesPlugin* rp = GetPlugin<ResourcesPlugin>();
	ResourcesBrowser* rb = rp->resourcesBrowser;
	
	rb->Close();
	rb->enableSelection();

	rb->selectGroup(group);
	int index = rb->ShowModal();
	rb->disableSelection();
	
	wxString selection = rb->getListCtrl()->GetItemText(index);
	String path = (String) selection.c_str();

	return GetResourceManager()->loadResource(path);
}

//-----------------------------------//

static ResourceHandle GetResourceUsingFile( ResourceGroup::Enum group )
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

	virtual bool OnButtonClick( wxPropertyGrid* propGrid, wxString& value ) OVERRIDE
	{
		PropertyData* data = (PropertyData*) GetClientObject();
		Class* resourceClass = (Class*) data->field->type;
		
		ResourceLoader* resourceLoader = GetResourceManager()->findLoaderByClass(resourceClass);
		if( !resourceLoader ) return false;

		ResourceHandle resource = GetResourceUsingBrowser( resourceLoader->getResourceGroup() );
		if( !resource ) return false;

		Resource* res = resource.Resolve();
		SetValueInEvent( PathGetFile(res->getPath()) );

		return true;
	}
};

//-----------------------------------//

PropertyPage::PropertyPage( wxWindow* parent )
	: wxPropertyGrid(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxPG_DEFAULT_STYLE | wxPG_SPLITTER_AUTO_CENTER)
	, currentObject(nullptr)
{
	// EventManager bindings.
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
	ClassWatchReset(&watch);
	Clear();
}

//-----------------------------------//

void PropertyPage::resetObject(const Object* object)
{
	if( currentObject == object )
		reset();
}

//-----------------------------------//

void PropertyPage::onIdle(wxIdleEvent& event)
{
	FieldWatchVector watches;
	ClassWatchUpdate(&watch, watches);
	
	if( watches.empty() ) return;

	for(size_t i = 0; i < watches.size(); i++)
	{
		FieldWatch* fw = watches[i];
		
		// Update value in the prop grid.
		wxAny value = getFieldValue(fw->field, (void*) fw->object);

		wxPGProperty* prop = (wxPGProperty*) fw->userdata;
		setPropertyValue(prop, value);
	}

	onClassFieldChanged(watches);
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
	propOperation->redo();
	
	UndoManager* undoManager = GetEditor().getDocument()->getUndoManager();
	undoManager->registerOperation(propOperation);
}

//-----------------------------------//

void PropertyPage::showProperties( Object* object, bool resetObject )
{
	if( resetObject ) reset();
	
	currentObject = object;
	
	Class* klass = ClassGetType(object);
	appendObjectFields(klass, object);
}

//-----------------------------------//

void PropertyPage::showEntityProperties( const EntityPtr& entity )
{
	currentObject = entity.get();

	// Entity properties.
	appendObjectFields( entity->getType(), entity.get() );

	// Transform properties.
	TransformPtr transform = entity->getTransform();

	if( transform )
		appendObjectFields( ReflectionGetType(Transform), transform.get() );

	// Other components properties.
	const ComponentMap& components = entity->getComponents();
	
	ComponentMap::const_iterator it;
	for( it = components.begin(); it != components.end(); it++ )
	{
		Class* type = it->first;
		const ComponentPtr& component = it->second;

		if( ReflectionIsEqual(type, ReflectionGetType(Transform)) )
			continue;

		appendObjectFields( type, component.get() );
	}
}

//-----------------------------------//

void PropertyPage::appendObjectFields(Class* klass, void* object, bool newCategory)
{
	if( newCategory )
	{
		const wxString& typeName = GetReadableName(klass->name);
		wxPropertyCategory* category = new wxPropertyCategory(typeName);
		Append(category);
	}

	if( klass->parent )
	{
		Class* parent = klass->parent;
		appendObjectFields(parent, object, false);
	}
	
	const std::vector<Field*>& fields = klass->fields;
	
	for( size_t i = 0; i < fields.size(); i++ )
	{
		const Field* field = fields[i];

		if( ReflectionIsComposite(field->type) && !ReflectionIsResourceHandle(field) )
		{
			void* addr = ClassGetFieldAddress(object, field);
			appendObjectFields((Class*) field->type, addr, false);
			continue;
		}

		wxPGProperty* prop = createProperty(*klass, *field, object);
		if(!prop) continue;

		Append(prop);
		Collapse(prop);

		wxAny value = getFieldValue(field, object);
		setPropertyValue(prop, value);

		FieldWatch fw;
		fw.object = object;
		fw.field = field;
		fw.userdata = prop;

		ClassWatchAddField(&watch, fw);
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

	if( !prop )
	{
		LogDebug("Could not create property for field: %s", field.name);
		return nullptr;
	}

	PropertyData* data = new PropertyData();
	data->type = &type;
	data->field = &field;
	data->object = object;

	prop->SetClientObject(data);

	wxString name = GetReadableName( field.name );
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
		wxString name = GetReadableName(it->first);
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
	default:
		assert(0);
	}

	return value;
}

//-----------------------------------//

void PropertyPage::setFieldValue(const Field* field, void* object, const wxAny& value)
{
	FieldWatch& fw = watch.fields[field];
	wxPGProperty* prop = (wxPGProperty*) fw.userdata;

	setPropertyValue(prop, value);
	//ClassWatchUpdateField(&watch, field);
}

//-----------------------------------//

void PropertyPage::setPropertyValue(wxPGProperty* prop, const wxAny& value)
{
	if( !prop )
	{
		LogDebug("Invalid property");
		return;
	}

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

NAMESPACE_EDITOR_END

#endif