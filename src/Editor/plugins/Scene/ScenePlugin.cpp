/************************************************************************
*
* vapor3D Editor © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "PCH.h"
#include "ScenePlugin.h"
#include "ScenePage.h"
#include "Editor.h"
#include "EditorIcons.h"
#include "Events.h"

namespace vapor { namespace editor {

//-----------------------------------//

ScenePlugin::ScenePlugin( EditorFrame* frame )
	: Plugin(frame)
	, scenePage(nullptr)
{ }

//-----------------------------------//

PluginMetadata ScenePlugin::getMetadata()
{
	PluginMetadata metadata;
	
	metadata.name = "Scene";
	metadata.description = "Provides a page with the scene contents";
	metadata.author = "triton";
	metadata.version = "1.0";

	return metadata;
}

//-----------------------------------//

void ScenePlugin::onPluginEnable()
{
	wxNotebook* notebookCtrl = editor->getNotebook();
	assert( notebookCtrl != nullptr );

	wxImageList* imageList = notebookCtrl->GetImageList();
	assert( imageList != nullptr );

	wxBitmap iconSitemap = wxMEMORY_BITMAP(sitemap_color);
	iconScene = imageList->Add(iconSitemap);

	scenePage = new ScenePage( editor, notebookCtrl );

	bool scenePageAdded = 
		notebookCtrl->AddPage( scenePage, "Scene", true, iconScene );

	if( !scenePageAdded )
		Log::warn( "Could not add page to notebook" );

	// Subscribe as an event listener.
	Events* events = editor->getEventManager();
	events->addEventListener(this);

	const ScenePtr& scene = engine->getSceneManager();
	scenePage->setScene(scene);
}

//-----------------------------------//

void ScenePlugin::onPluginDisable()
{
	removePage( scenePage );

	// Unsubscribe as an event listener.
	Events* events = editor->getEventManager();
	events->removeEventListener(this);
}

//-----------------------------------//

void ScenePlugin::onNodeSelect( const NodePtr& node )
{
	if( scenePage->sentLastSelectionEvent )
		return;

	wxTreeItemId nodeId = scenePage->getTreeIdFromNode(node);

	if( !nodeId.IsOk() )
		return;

	//wxTreeCtrl* treeCtrl = scenePage->getTreeCtrl();
	//treeCtrl->SelectItem(nodeId);
}

//-----------------------------------//

void ScenePlugin::onNodeUnselect( const NodePtr& node )
{
	if( scenePage->sentLastSelectionEvent )
		return;

	wxTreeCtrl* treeCtrl = scenePage->getTreeCtrl();
	treeCtrl->Unselect();
}

//-----------------------------------//

void ScenePlugin::onSceneLoad( const ScenePtr& scene )
{
	scenePage->setScene(scene);
}

//-----------------------------------//

} } // end namespaces