/************************************************************************
*
* vapor3D Editor © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "PCH.h"
#include "SelectionManager.h"
#include "Editor.h"
#include "UndoManager.h"
#include "Events.h"

namespace vapor { namespace editor {

//-----------------------------------//

SelectionManager::SelectionManager( EditorFrame* editor )
	: mode(SelectionMode::Entity)
	, selection(nullptr)
{
	events = editor->getEventManager();
	undoManager = editor->getUndoManager();
}

//-----------------------------------//

SelectionMode::Enum SelectionManager::getSelectionMode() const
{
	return mode;
}

//-----------------------------------//

void SelectionManager::setSelectionMode(SelectionMode::Enum mode)
{
	this->mode = mode;
}

//-----------------------------------//

SelectionOperation* SelectionManager::getSelection() const
{
	return selection;
}

//-----------------------------------//

void SelectionManager::setSelection(SelectionOperation* selection)
{
	this->selection = selection;
}

//-----------------------------------//

SelectionOperation* SelectionManager::createOperation()
{
	SelectionOperation* selection = new SelectionOperation();
	
	selection->mode = mode;
	selection->events = events;
	selection->selectionManager = this;

	return selection;
}

//-----------------------------------//

SelectionOperation::~SelectionOperation()
{
	if( selectionManager->getSelection() == this )
		selectionManager->setSelection(nullptr);
}

//-----------------------------------//

/** 
Unselect everything
	Undo: Select previous selection
	Redo: Unselect previous selection

Select with no current selection
	Undo: Unselect selection
	Redo: Select selection
	
Select with another previous selection
	Undo: Unselect selection && Select previous selection
	Redo: Select selection && Unselect previous selection */

void SelectionOperation::undo()
{
	if( mode == SelectionMode::None )
	{
		selectPrevious();
	}
	else if( previous.empty() )
	{
		unselectAll();
	}
	else
	{
		unselectAll();
		selectPrevious();
	}

	lastUndone = true;
	selectionManager->setSelection(this);
}

//-----------------------------------//

void SelectionOperation::redo()
{
	if( mode == SelectionMode::None )
	{
		unselectPrevious();
	}
	else if( previous.empty() )
	{
		selectAll();
	}
	else
	{
		unselectPrevious();
		selectAll();
	}

	lastUndone = false;
	selectionManager->setSelection(this);
}

//-----------------------------------//

void SelectionOperation::selectAll()
{
	for(uint i = 0; i < selections.size(); i++ )
	{
		const SelectionData& data = selections[i];
		const EntityPtr& entity = data.entity;

		setBoundingBoxVisible(entity, true);
		events->onEntitySelect(entity);
	}
}

//-----------------------------------//

void SelectionOperation::unselectAll()
{
	for(uint i = 0; i < selections.size(); i++ )
	{
		const SelectionData& data = selections[i];
		const EntityPtr& entity = data.entity;

		setBoundingBoxVisible(entity, false);
		events->onEntityUnselect(entity);
	}
}

//-----------------------------------//

void SelectionOperation::selectPrevious()
{
	for(uint i = 0; i < previous.size(); i++ )
	{
		const SelectionData& data = previous[i];
		const EntityPtr& entity = data.entity;

		setBoundingBoxVisible(entity, true);
		events->onEntitySelect(entity);
	}
}

//-----------------------------------//

void SelectionOperation::unselectPrevious()
{
	for(uint i = 0; i < previous.size(); i++ )
	{
		const SelectionData& data = previous[i];
		const EntityPtr& entity = data.entity;

		setBoundingBoxVisible(entity, false);
		events->onEntityUnselect(entity);
	}
}

//-----------------------------------//

void SelectionOperation::addEntity(const EntityPtr& entity)
{
	SelectionData data;
	data.entity = entity;

	selections.push_back(data);
}

//-----------------------------------//

bool SelectionOperation::isSelection(const EntityPtr& entity)
{
	for(uint i = 0; i < selections.size(); i++)
	{
		SelectionData& data = selections[i];
		
		if(data.entity == entity)
			return true;
	}

	return false;
}

//-----------------------------------//

void SelectionOperation::setBoundingBoxVisible(const EntityPtr& entity, bool state)
{
	if( !entity )
		return;

	const TransformPtr& transform = entity->getTransform();

	if( !transform )
		return;

	transform->setDebugRenderableVisible( state );
}

//-----------------------------------//

} } // end namespaces