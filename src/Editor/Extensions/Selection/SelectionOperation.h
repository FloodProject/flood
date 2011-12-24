/************************************************************************
*
* vapor3D Editor © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "UndoOperation.h"
#include "SelectionManager.h"

NAMESPACE_EDITOR_BEGIN

//-----------------------------------//

REFLECT_DECLARE_CLASS(SelectionOperation)

class SelectionOperation : public UndoOperation
{
	REFLECT_DECLARE_OBJECT(SelectionOperation)
	friend class SelectionManager;

public:

	SelectionOperation();

	// Undoes/redoes the operation.
	void undo() OVERRIDE;	
	void redo() OVERRIDE;

	// Sets the previous selections.
	void setPreviousSelections(SelectionCollection);

	// Selections.
	SelectionCollection selections;
	
	// Previous selections.
	SelectionCollection previous;

	bool isDeselection;
};

SelectionOperation* CreateSelectionOperation(const String& desc);
SelectionOperation* CreateEntitySelectionOperation(const EntityPtr& entity);

TYPEDEF_INTRUSIVE_POINTER_FROM_TYPE(SelectionOperation)

//-----------------------------------//

NAMESPACE_EDITOR_END