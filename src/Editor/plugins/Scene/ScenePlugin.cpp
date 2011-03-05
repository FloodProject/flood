/************************************************************************
*
* vapor3D Editor © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "PCH.h"
#include "ScenePlugin.h"
#include "ScenePane.h"
#include "Editor.h"
#include "EditorIcons.h"
#include "Events.h"
#include "Engine.h"

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
	scenePage = new ScenePage(editor);
	scenePage->SetSize(180, -1);

	wxBitmap icon = wxMEMORY_BITMAP(sitemap_color);

	wxAuiPaneInfo pane;
	pane.Caption("Hierarchy").Name("Hierarchy").Right().Hide().Dock().Icon(icon);
	pane.MinSize( scenePage->GetSize() );

	editor->getAUI()->AddPane(scenePage, pane);
	editor->getAUI()->Update();

	// Subscribe as an event listener.
	Events* events = editor->getEventManager();
	events->addEventListener(this);
}

//-----------------------------------//

void ScenePlugin::onPluginDisable()
{
	//removePage( scenePage );
	editor->getAUI()->DetachPane(scenePage);
	editor->getAUI()->Update();

	delete scenePage;
	scenePage = nullptr;

	// Unsubscribe as an event listener.
	Events* events = editor->getEventManager();
	events->removeEventListener(this);
}

//-----------------------------------//

void ScenePlugin::onEntitySelect( const EntityPtr& entity )
{
	if( scenePage->sentLastSelectionEvent )
		return;

	wxTreeItemId entityId = scenePage->getTreeIdFromEntity(entity);

	if( !entityId.IsOk() )
		return;
}

//-----------------------------------//

void ScenePlugin::onEntityUnselect( const EntityPtr& entity )
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