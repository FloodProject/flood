/************************************************************************
*
* vapor3D Editor © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "PCH.h"
#include "SelectionPlugin.h"
#include "Editor.h"

namespace vapor { namespace editor {

//-----------------------------------//

SelectionPlugin::SelectionPlugin( EditorFrame* frame )
	: Plugin(frame)
{
}

//-----------------------------------//

PluginMetadata SelectionPlugin::getMetadata()
{
	PluginMetadata metadata;
	
	metadata.name = "Selection";
	metadata.description = "Provides selection services";
	metadata.author = "triton";
	metadata.version = "1.0";

	return metadata;
}

//-----------------------------------//

void SelectionPlugin::onPluginEnable()
{
	// Connect to undo/redo events.
	//undoManager->onUndoRedoEvent.Connect(this, &UndoPlugin::onUndoEvent);
}

//-----------------------------------//

void SelectionPlugin::onPluginDisable()
{
	// Disconnect to undo/redo events.
	//undoManager->onUndoRedoEvent.Connect(this, &UndoPlugin::onUndoEvent);
}

//-----------------------------------//

} } // end namespaces