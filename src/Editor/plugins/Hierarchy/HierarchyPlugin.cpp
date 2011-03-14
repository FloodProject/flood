/************************************************************************
*
* vapor3D Editor © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Editor/API.h"
#include "HierarchyPlugin.h"
#include "HierarchyPane.h"
#include "Editor.h"
#include "EditorIcons.h"
#include "Events.h"

namespace vapor { namespace editor {

//-----------------------------------//

REFLECT_CHILD_CLASS(HierarchyPlugin, Plugin)
REFLECT_END()

//-----------------------------------//

HierarchyPlugin::HierarchyPlugin()
	: hierarchyPane(nullptr)
{ }

//-----------------------------------//

PluginMetadata HierarchyPlugin::getMetadata()
{
	PluginMetadata metadata;
	
	metadata.name = "Hierarchy";
	metadata.description = "Displays hierarchy information";
	metadata.author = "triton";
	metadata.version = "1.0";
	metadata.startEnabled = false;

	return metadata;
}

//-----------------------------------//

void HierarchyPlugin::onPluginEnable()
{
	hierarchyPane = new HierarchyPane(editor);
	hierarchyPane->SetSize(200, -1);

	wxBitmap icon = wxMEMORY_BITMAP(sitemap_color);

	wxAuiPaneInfo pane;
	pane.Caption("Hierarchy").Right().Dock().Icon(icon);
	pane.MinSize( hierarchyPane->GetSize() );

	editor->getAUI()->AddPane(hierarchyPane, pane);
	editor->getAUI()->Update();

	// Subscribe as an event listener.
	Events* events = editor->getEventManager();
	events->addEventListener(this);
}

//-----------------------------------//

void HierarchyPlugin::onPluginDisable()
{
	editor->getAUI()->DetachPane(hierarchyPane);
	editor->getAUI()->Update();

	delete hierarchyPane;
	hierarchyPane = nullptr;

	// Unsubscribe as an event listener.
	Events* events = editor->getEventManager();
	events->removeEventListener(this);
}

//-----------------------------------//

} } // end namespaces