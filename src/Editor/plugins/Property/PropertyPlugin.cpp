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
	propertyPage->showNodeProperties( node );

	Class& klass = (Class&) node->getInstanceType();
	klass.onFieldChanged += fd::bind(&PropertyPlugin::onFieldChanged, this);
}

//-----------------------------------//

void PropertyPlugin::onNodeUnselect( const NodePtr& node )
{
	selectedNode.reset();
	propertyPage->Clear();

	Class& klass = (Class&) node->getInstanceType();
	klass.onFieldChanged -= fd::bind(&PropertyPlugin::onFieldChanged, this);
}

//-----------------------------------//

void PropertyPlugin::onFieldChanged(const Field& field)
{
	const NodePtr& node = selectedNode.lock();

	if( node )	
		propertyPage->showNodeProperties(node);
}

//-----------------------------------//

void PropertyPlugin::onSceneUpdate()
{
	const NodePtr& node = selectedNode.lock();

	if( node )
		propertyPage->showNodeProperties( node );
}

//-----------------------------------//

} } // end namespaces