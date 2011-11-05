/************************************************************************
*
* vapor3D Editor © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Editor/API.h"
#include "SelectionManager.h"
#include "SelectionOperation.h"

#include "Editor.h"
#include "UndoManager.h"
#include "EventManager.h"

#include "Scene/Entity.h"
#include "Scene/Transform.h"

NAMESPACE_EDITOR_BEGIN

//-----------------------------------//

SelectionData::SelectionData()
	: mode(SelectionMode::None)
{
}

//-----------------------------------//

void SelectionCollection::addEntity(const EntityPtr& entity)
{
	SelectionData data;
	
	data.mode = SelectionMode::Entity;
	data.entity = entity;

	selections.push_back(data);
}

//-----------------------------------//

bool SelectionCollection::hasEntity(const EntityPtr& entity) const
{
	for(size_t i = 0; i < selections.size(); i++)
	{
		const SelectionData& data = selections[i];
		if(data.entity == entity) return true;
	}

	return false;
}

//-----------------------------------//

bool SelectionCollection::isEmpty() const
{
	return selections.empty();
}

//-----------------------------------//

bool SelectionCollection::isSame(const SelectionCollection& selection) const
{
	const SelectionsVector& sels = selection.selections;
	bool sameSize = sels.size() == selections.size();

	return contains(selection) && sameSize;
}

//-----------------------------------//

bool SelectionCollection::contains(const SelectionData& selection) const
{
	auto it = std::find(selections.begin(), selections.end(), selection);
	return it != selections.end();
}

//-----------------------------------//

bool SelectionCollection::contains(const SelectionCollection& selection) const
{
	const SelectionsVector& sels = selection.selections;

	bool isEmpty = sels.empty() || selections.empty();
	
	if( isEmpty )
		return sels.empty() && selections.empty();
	
	if( selection.isEmpty() ) return false;

	
	for(size_t i = 0; i < sels.size(); i++)
	{
		const SelectionData& data = sels[i];
		if( !contains(data) ) return false;
	}

	return true;
}

//-----------------------------------//

SelectionManager::SelectionManager()
	: mode(SelectionMode::Entity)
{
	dragRectangle = nullptr;
}

//-----------------------------------//

bool SelectionData::operator == (const SelectionData& rhs) const
{
	switch(mode)
	{
	case SelectionMode::Entity:
		return entity == rhs.entity;
	default:
		LogAssert("Unsupported selection mode");
	}

	return false;
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

void SelectionManager::addSelection(const SelectionData& selection)
{
	// Add the selection to the current selections.
	selections.selections.push_back(selection);

	EventManager* events = GetEditor().getEventManager();

	switch(selection.mode)
	{
	case SelectionMode::Entity:
	{
		const EntityPtr& entity = selection.entity;
		events->onEntitySelect(entity);
		break;
	}
	default:
		LogAssert("Unsupported selection mode");
		return;
	}
}

//-----------------------------------//

void SelectionManager::removeSelection(const SelectionData& selection)
{
	// Search the current selections for the selection.
	SelectionsVector& sels = selections.selections;

	if( std::find(sels.begin(), sels.end(), selection) == sels.end() )
	{
		// Selection was not found in the current selections.
		return;
	}

	// Remove the selection from the current selections.
	auto it = std::remove(sels.begin(), sels.end(), selection);
	sels.erase(it, sels.end());

	EventManager* events = GetEditor().getEventManager();

	switch(selection.mode)
	{
	case SelectionMode::Entity:
	{
		const EntityPtr& entity = selection.entity;
		events->onEntityUnselect(entity);
		break;
	}
	default:
		LogAssert("Unsupported selection mode");
		return;
	}
}

//-----------------------------------//

void SelectionManager::addSelections(const SelectionCollection& selections)
{
	const SelectionsVector& sels = selections.selections;
	
	for(size_t i = 0; i < sels.size(); i++)
	{
		const SelectionData& sel = sels[i];
		addSelection(sel);
	}
}

//-----------------------------------//

void SelectionManager::removeSelections(const SelectionCollection& selections)
{
	const SelectionsVector& sels = selections.selections;
	
	for(size_t i = 0; i < sels.size(); i++)
	{
		const SelectionData& sel = sels[i];
		removeSelection(sel);
	}
}

//-----------------------------------//

void SelectionManager::removeCurrent()
{
	removeSelections(selections);
}

//-----------------------------------//

const SelectionCollection& SelectionManager::getSelections() const
{
	return selections;
}

//-----------------------------------//

NAMESPACE_EDITOR_END