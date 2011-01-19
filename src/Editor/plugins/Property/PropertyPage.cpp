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

struct TagName
{
	long id;
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

	for( uint i = 0; i < VAPOR_ARRAY_SIZE(TagNames); i++ )
	{
		const TagName& tag = TagNames[i];
			
		wxString name( std::string(tag.name) );
		choices.Add( convertToReadable(name), tag.id );
	}

	return choices;
}

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

void PropertyPage::reset()
{
	selectedEntity.reset();
	memoryWatches.clear();
	Clear();
}

//-----------------------------------//

void PropertyPage::onIdle(wxIdleEvent& event)
{
	if( !updateMemoryWatches() )
		return;

	Log::debug("Memory watches detected changes");
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
	
	if(	!prop->GetClientObject() )
		prop = prop->GetParent();

	PropertyData* data = (PropertyData*) prop->GetClientObject();

	PropertyOperation* op = new PropertyOperation();
	op->description = "Property changed";
	op->type = (Class*) data->type;
	op->field = (Field*) data->field;
	op->object = (void*) data->object;
	op->oldValue = propertyValue;
	op->newValue = getPropertyValue(prop);
	op->grid = this;
	
	UndoManager* undoManager = GetEditor().getUndoManager();
	undoManager->registerOperation(op);

	op->redo();
}

//-----------------------------------//

void PropertyPage::showEntityProperties( const EntityPtr& entity )
{
	reset();
	selectedEntity = entity;

    // Entity properties.
	appendObjectFields( entity->getType(), entity.get() );

	// Transform properties.
	TransformPtr transform = entity->getTransform();

	if( transform )
		appendObjectFields( transform->getType(), transform.get() );
    
    // Other components properties.
	const ComponentMap& components = entity->getComponents();
	
	ComponentMap::const_iterator it;
	for( it = components.cbegin(); it != components.cend(); it++ )
	{
		const Class& type = *(it->first);
		const ComponentPtr& component = it->second;

		if( type.is<Transform>() )
			continue;

		appendObjectFields( type, component.get() );
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
		if(!prop) continue;

		Append(prop);

#if 1
		for( uint i = 0; i < composed.size(); i++ )
		{
			wxPGProperty* cp = composed[i];
			AppendIn( prop, cp );
		}

		composed.clear();
#endif

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

	wxString name = convertToReadable( field.name );
	prop->SetLabel( name );

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
	const Enum& type = (const Enum&) field.type;
	const EnumValuesMap& values = type.getValues();
	
	wxPGChoices choices;
	
	EnumValuesMap::const_iterator it;
	for( it = values.cbegin(); it != values.cend(); it++ )
		choices.Add( it->first, it->second );

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
	const Primitive& type = (const Primitive&) field.type;

	if( type.isBool() )
	{
		prop = new wxBoolProperty( wxEmptyString, wxPG_LABEL );
	}
	//-----------------------------------//
	else if( type.isInteger() )
	{
		prop = new wxIntProperty( wxEmptyString, wxPG_LABEL );
	}
	//-----------------------------------//
	else if( type.isFloat() )
	{
		prop = createFloatProperty( (const char*) wxEmptyString, 0 );
	}
	//-----------------------------------//
	else if( type.isString() )
	{
		prop = new wxStringProperty( wxEmptyString, wxPG_LABEL );
	}
	//-----------------------------------//
	else if( type.isColor() )
	{
		prop = new wxColourProperty( wxEmptyString, wxPG_LABEL );
	}
	//-----------------------------------//
	else if( type.isVector3() )
	{
		prop = new wxStringProperty( wxEmptyString, wxPG_LABEL, "<composed>" );
		composed.push_back( createFloatProperty("X", 0) );
		composed.push_back( createFloatProperty("Y", 0) );
		composed.push_back( createFloatProperty("Z", 0) );
	}
	//-----------------------------------//
	else if( type.isQuaternion() )
	{
		prop = new wxStringProperty( wxEmptyString, wxPG_LABEL, "<composed>" );
		composed.push_back( createFloatProperty("X", 0) );
		composed.push_back( createFloatProperty("Y", 0) );
		composed.push_back( createFloatProperty("Z", 0) );
	}
	//-----------------------------------//
	else if( type.isBitfield() )
	{
		wxPGChoices choices = getTagChoices();
		prop = new wxFlagsProperty( wxEmptyString, wxPG_LABEL, choices );
	}
	//-----------------------------------//
	else
	{
		Log::debug( "Unknown property type: '%s'", type.getName().c_str() );
	}

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

	bool isResource = field->type.inherits<Resource>();

	if( field->isPointer() && isResource )
	{
		std::string name;
		ResourcePtr res = field->get<ResourcePtr>(object);
		if(res) name = res->getPath();
		value = Path::getFile(name);
	}
	else if( field->type.isEnum() )
		value = field->get<int>(object);
	else if( field->type.isPrimitive() )
		value = getFieldPrimitiveValue(field, object);

	return value;
}

//-----------------------------------//

wxAny PropertyPage::getFieldPrimitiveValue(const Field* field, void* object)
{
	wxAny value;

	const Primitive& type = (const Primitive&) field->type;

	if( type.isBool() )
		value = field->get<bool>(object);
	else if( type.isInteger() )
		value = field->get<int>(object);
	else if( type.isFloat() )
		value = field->get<float>(object);
	else if( type.isString() )
		value = field->get<std::string>(object);
	else if( type.isColor() )
		value = convertColor(field->get<Color>(object));
	else if( type.isVector3() )
		value = field->get<Vector3>(object);
	else if( type.isQuaternion() )
		value = field->get<Quaternion>(object).getEulerAngles();
	else if( type.isBitfield() )
		value = field->get<int>(object);

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
	if(	!prop->GetClientObject() )
		prop = prop->GetParent();

	PropertyData* data = (PropertyData*) prop->GetClientObject();
	
	const Type& type = data->field->type;
	bool isResource = type.inherits<Resource>();
	
	if( data->field->isPointer() && isResource )
	{
		std::string res = value.As<std::string>();
		prop->SetValue(res);
	}
	else if( type.isEnum() )
	{
		int val = value.As<int>();
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
	if(	!prop->GetClientObject() )
		prop = prop->GetParent();

	PropertyData* data = (PropertyData*) prop->GetClientObject();

	const Primitive& type = (Primitive&) data->field->type;

	if( type.isBool() )
	{
		bool val = value.As<bool>();
		prop->SetValue(val);
	}
	//-----------------------------------//
	else if( type.isInteger() )
	{
		int val = value.As<int>();
		prop->SetValue(val);
	}
	//-----------------------------------//
	else if( type.isFloat() )
	{
		float val = value.As<float>();
		prop->SetValue(val);
	}
	//-----------------------------------//
	else if( type.isString() )
	{
		std::string val = value.As<std::string>();
		prop->SetValue(val);
	}
	//-----------------------------------//
	else if( type.isColor() )
	{
		Color val = value.As<Color>();
		prop->SetValue( wxAny(convertColor(val)) );
	}
	//-----------------------------------//
	else if( type.isVector3() )
	{
		Vector3 vec = value.As<Vector3>();
		prop->GetPropertyByName("X")->SetValue(vec.x);
		prop->GetPropertyByName("Y")->SetValue(vec.y);
		prop->GetPropertyByName("Z")->SetValue(vec.z);
	}
	//-----------------------------------//
	else if( type.isQuaternion() )
	{
		EulerAngles vec = value.As<EulerAngles>();
		prop->GetPropertyByName("X")->SetValue(vec.x);
		prop->GetPropertyByName("Y")->SetValue(vec.y);
		prop->GetPropertyByName("Z")->SetValue(vec.z);
	}
	//-----------------------------------//
	else if( type.isBitfield() )
	{
		int bits = value.As<int>();
		prop->SetValue(bits);
	}
}

//-----------------------------------//

wxAny PropertyPage::getPropertyValue(wxPGProperty* prop)
{
	if(	!prop->GetClientObject() )
		prop = prop->GetParent();

	PropertyData* data = (PropertyData*) prop->GetClientObject();

	const Type& type = data->field->type;
	bool isResource = type.inherits<Resource>();
	
	if( data->field->isPointer() && isResource )
	{
		return (std::string) prop->GetValue(); 
	}
	else if( type.isEnum() )
	{
		return prop->GetValue().GetAny().As<int>();
	}
	else if( type.isPrimitive() )
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
	const Primitive& type = (const Primitive&) data->field->type;

	bool isVector = type.isVector3();
	bool isQuaternion = type.isQuaternion();
		
	if( type.isBool() )
	{
		bool val = prop->GetValue().GetAny().As<bool>();
		return val;
	}
	else if( type.isInteger() )
	{
		int val = prop->GetValue().GetAny().As<int>();
		return val;
	}
	else if( type.isFloat() )
	{
		float val = prop->GetValue().GetAny().As<float>();
		return val;
	}
	else if( type.isString() )
	{
		std::string val = prop->GetValue();
		return val;
	}
	else if( type.isColor() )
	{
		wxColour val = prop->GetValue();
		return convertColorFromWx(val);
	}
	else if( type.isBitfield() )
	{
		int bits = prop->GetValue().GetAny().As<int>();
		return bits;
	}
	else if( isVector || isQuaternion )
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

	return prop->GetValue();
}

//-----------------------------------//

} } // end namespaces