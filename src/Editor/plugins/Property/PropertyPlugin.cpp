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

	wxBitmap iconProp = wxMEMORY_BITMAP(application_view_list);
	iconProperty = imageList->Add(iconProp);

	wxBoxSizer* sizer = new wxBoxSizer( wxVERTICAL );

	wxPanel* panel = new wxPanel(notebookCtrl);
	panel->SetSizer(sizer);

	propertyPage = new PropertyPage( editor, panel );
	sizer->Add( propertyPage, wxSizerFlags(1).Expand() );

	bool propertyPageAdded = notebookCtrl->AddPage( panel,
		wxEmptyString/*wxT("Property")*/, false, iconProperty );

	if( !propertyPageAdded )
		Log::warn( "Could not add page to notebook" );

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

void PropertyPlugin::onEntitySelect( const EntityPtr& node )
{
	selectedEntity = node;
	propertyPage->showEntityProperties( node );

	Class& klass = (Class&) node->getInstanceType();
	klass.onFieldChanged.Connect(this, &PropertyPlugin::onFieldChanged);

	node->onComponentAdded.Connect(this, &PropertyPlugin::onComponentChanged);
	node->onComponentRemoved.Connect(this, &PropertyPlugin::onComponentChanged);
}

//-----------------------------------//

void PropertyPlugin::onEntityUnselect( const EntityPtr& node )
{
	selectedEntity.reset();
	propertyPage->Clear();

	Class& klass = (Class&) node->getInstanceType();
	klass.onFieldChanged.Disconnect(this, &PropertyPlugin::onFieldChanged);

	node->onComponentAdded.Disconnect(this, &PropertyPlugin::onComponentChanged);
	node->onComponentRemoved.Disconnect(this, &PropertyPlugin::onComponentChanged);
}

//-----------------------------------//

void PropertyPlugin::onComponentSelect( const ComponentPtr& component )
{
	const EntityPtr& node = component->getEntity();
	onEntitySelect(node);
}

//-----------------------------------//

void PropertyPlugin::onComponentUnselect( const ComponentPtr& component )
{
	const EntityPtr& node = component->getEntity();
	onEntityUnselect(node);
}

//-----------------------------------//

void PropertyPlugin::onFieldChanged(const Field& field)
{
	updateProperties();
}

//-----------------------------------//

void PropertyPlugin::onComponentChanged(const ComponentPtr& component)
{
	updateProperties();
}

//-----------------------------------//

void PropertyPlugin::onSceneUpdate()
{
	updateProperties();
}

//-----------------------------------//

void PropertyPlugin::updateProperties()
{
	const EntityPtr& node = selectedEntity.lock();

	if( node )	
		propertyPage->showEntityProperties(node);
}

//-----------------------------------//

} } // end namespaces