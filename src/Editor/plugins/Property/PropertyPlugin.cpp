/************************************************************************
*
* vapor3D Editor © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "PCH.h"
#include "PropertyPlugin.h"
#include "PropertyPage.h"
#include "Editor.h"
#include "Events.h"
#include "EditorIcons.h"

#include <wx/propgrid/property.h>
#include <wx/propgrid/advprops.h>

namespace vapor { namespace editor {

//-----------------------------------//

PropertyPlugin::PropertyPlugin( EditorFrame* frame )
	: Plugin(frame)
	, propertyPage(nullptr)
{ }

//-----------------------------------//

PluginMetadata PropertyPlugin::getMetadata()
{
	PluginMetadata metadata;
	
	metadata.name = "Property";
	metadata.description = "Provides a page with object properties";
	metadata.author = "triton";
	metadata.version = "1.0";

	return metadata;
}

//-----------------------------------//

void PropertyPlugin::onPluginEnable()
{
	wxNotebook* notebookCtrl = editor->getNotebook();
	assert( notebookCtrl != nullptr );

	wxImageList* imageList = notebookCtrl->GetImageList();
	assert( imageList != nullptr );

	wxBitmap iconPackage = wxMEMORY_BITMAP(grid_icon);
	iconProperty = imageList->Add(iconPackage);

	propertyPage = new PropertyPage( editor, notebookCtrl );

	bool propertyPageAdded = notebookCtrl->AddPage( propertyPage,
		wxEmptyString/*wxT("Property")*/, false, iconProperty );

	if( !propertyPageAdded )
		warn( "editor", "Could not add page to notebook" );

	notebookCtrl->Refresh();

	// Subscribe as an event listener.
	Events* events = editor->getEventManager();
	events->addEventListener(this);
}

//-----------------------------------//

void PropertyPlugin::onPluginDisable()
{
	removePage( propertyPage );

	// Unsubscribe as an event listener.
	Events* events = editor->getEventManager();
	events->removeEventListener(this);
}

//-----------------------------------//

void PropertyPlugin::onNodeSelect( const NodePtr& node )
{
	selectedNode = node;
	showNodeProperties( node );
}

//-----------------------------------//

void PropertyPlugin::onNodeUnselect( const NodePtr& )
{
	selectedNode.reset();

	wxPropertyGrid* pg = propertyPage;
	pg->Clear();
}

//-----------------------------------//

void PropertyPlugin::onSceneUpdate()
{
	NodePtr node( selectedNode );

	if( node )
		showNodeProperties( node );
}

//-----------------------------------//

void PropertyPlugin::showNodeProperties( const NodePtr& node )
{
	wxPropertyGrid* pg = propertyPage;
	pg->Clear();

    // Node properties
	appendObjectFields( node->getInstanceType(), node.get() );
    
    // Components properties
	foreach( const ComponentMapPair& p, node->getComponents() )
	{
		const Class& type = *(p.first);
		const ComponentPtr& component = p.second;

		appendObjectFields(type, component.get() );
	}
}

//-----------------------------------//

void PropertyPlugin::appendObjectFields(const Class& type, void* object)
{
	wxPropertyGrid* pg = propertyPage;

	pg->Append( new wxPropertyCategory( type.getName() ) );

	foreach( const FieldsPair& p, type.getFields() )
	{
		const Field& field = *p.second;
		const Type& type = field.type;

		wxPGProperty* prop = nullptr;

		if( type.isPrimitive() )
		{
			prop = createPrimitiveProperty(field, object);
		}
		else if( type.isEnum() )
		{
			prop = createEnumProperty(field, object);
		}
		else if( type.isClass() || type.isStruct() )
		{
			appendObjectFields((Class&) type, object);
		}
		else
		{
			assert( false );
		}

		wxString name = convertToReadable(p.first);
		prop->SetLabel( name );
	}
}

//-----------------------------------//

wxPGProperty* PropertyPlugin::createEnumProperty(const Field& field, void* object)
{
	wxPropertyGrid* pg = propertyPage;
	wxPGProperty* prop = nullptr;

	assert( field.type.isEnum() );
	const Enum& type = (const Enum&) field.type;

	wxPGChoices arrValues;

	foreach( const EnumValuesPair& p, type.getValues() )
	{
		arrValues.Add( p.first, p.second );
	}

	prop = new wxEnumProperty( wxEmptyString, wxPG_LABEL, arrValues );
	pg->Append( prop );

	return prop;
}

//-----------------------------------//

wxPGProperty* PropertyPlugin::createPrimitiveProperty(const Field& field, void* object)
{
	wxPGProperty* prop = nullptr;
	wxPropertyGrid* pg = propertyPage;

	assert( field.type.isPrimitive() );
	const Primitive& type = (const Primitive&) field.type;

	if( type.isBool() )
	{
		bool val = field.get<bool>(object);
		prop = new wxBoolProperty( wxEmptyString, wxPG_LABEL, val );
		pg->Append( prop );
	}

	else if( type.isInteger() )
	{
		int val = field.get<int>(object);
		prop = new wxIntProperty( wxEmptyString, wxPG_LABEL, val );
		pg->Append( prop );
	}

	else if( type.isFloat() )
	{
		float val = field.get<float>(object);
		prop = new wxFloatProperty( wxEmptyString, wxPG_LABEL, val );
		pg->Append( prop );
	}

	else if( type.isString() )
	{
		std::string val = field.get<std::string>(object);
		prop = new wxStringProperty( wxEmptyString, wxPG_LABEL, val );
		pg->Append( prop );
	}

	else if( type.isColor() )
	{
		Color val = field.get<Color>(object);
		wxColour wx( val.r*255, val.g*255, val.b*255, val.a*255 );
		prop = new wxColourProperty( wxEmptyString, wxPG_LABEL, wx );
		pg->Append( prop );
	}

	else if( type.isVector3() )
	{
		Vector3 vec = field.get<Vector3>(object);
		//std::string str = String::format( "%.3f, %.3f, %.3f", vec.x, vec.y, vec.z );
		
		prop = new wxArrayStringProperty( wxEmptyString, wxPG_LABEL );
		pg->Append( prop );

		pg->AppendIn( prop, new wxFloatProperty( "X", wxPG_LABEL, vec.x ) );
		pg->AppendIn( prop, new wxFloatProperty( "Y", wxPG_LABEL, vec.y ) );
		pg->AppendIn( prop, new wxFloatProperty( "Z", wxPG_LABEL, vec.z ) );
	}

	if( !prop )
	{
		debug( "Unknown property type: '%s'", type.getName().c_str() );
		prop = new wxStringProperty();
	}

	return prop;
}

//-----------------------------------//

wxString PropertyPlugin::convertToReadable(const wxString& str)
{
	if( str.IsEmpty() ) return wxEmptyString;

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

} } // end namespaces