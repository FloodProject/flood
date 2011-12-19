/************************************************************************
*
* vapor3D Editor © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Editor/API.h"
#include "SelectionOperation.h"
#include "SelectionPlugin.h"
#include "Editor.h"

NAMESPACE_EDITOR_BEGIN

//-----------------------------------//

REFLECT_ABSTRACT_CHILD_CLASS(SelectionOperation, UndoOperation)
REFLECT_CLASS_END()

SelectionOperation::SelectionOperation()
	: isDeselection(false)
{
}

//-----------------------------------//

/**
Unselect everything
	Undo: Select previous selection
	Redo: Unselect previous selection

Select with no previous selection
	Undo: Unselect selection
	Redo: Select selection
	
Select with previous selection
	Undo: Unselect selection && Select previous selection
	Redo: Select selection && Unselect previous selection */

void SelectionOperation::undo()
{
	SelectionPlugin* plugin = GetPlugin<SelectionPlugin>();
	SelectionManager* selection = plugin->getSelectionManager();

	if( isDeselection )
	{
		selection->addSelections(previous);
	}
	else
	{
		selection->removeSelections(selections);
		selection->addSelections(previous);
	}

	lastUndone = true;
}

//-----------------------------------//

void SelectionOperation::redo()
{
	SelectionPlugin* plugin = GetPlugin<SelectionPlugin>();
	SelectionManager* selection = plugin->getSelectionManager();


	if( isDeselection )
	{
		selection->removeSelections(previous);
	}
	else
	{
		selection->removeSelections(previous);
		selection->addSelections(selections);
	}

	lastUndone = false;
}

//-----------------------------------//

void SelectionOperation::setPreviousSelections(SelectionCollection selections)
{
	assert( previous.isEmpty() );
	previous = selections;
}

//-----------------------------------//

SelectionOperation* CreateSelectionOperation(const String& desc)
{
	SelectionOperation* selection = AllocateHeap(SelectionOperation);
	selection->description = desc;

	return selection;
}

//-----------------------------------//

SelectionOperation* CreateEntitySelectionOperation(const EntityPtr& entity)
{
	SelectionManager* selections = GetPlugin<SelectionPlugin>()->getSelectionManager();
	if( !selections ) return nullptr;

	SelectionOperation* selection = CreateSelectionOperation("Entity selection");
	selection->selections.addEntity(entity);
	selection->setPreviousSelections( selections->getSelections() );

	return selection;
}

//-----------------------------------//

NAMESPACE_EDITOR_END