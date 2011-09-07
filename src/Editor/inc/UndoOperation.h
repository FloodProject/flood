/************************************************************************
*
* vapor3D Editor © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "Core/References.h"

NAMESPACE_EDITOR_BEGIN

//-----------------------------------//

/**
 * Represents an editing operation that can be undone and redone.
 * This is the base class of the undo/redo system and every operation
 * that the engine provides should be provided using this type.
 */

REFLECT_DECLARE_CLASS(UndoOperation)

class UndoOperation : public Object
{
	REFLECT_DECLARE_OBJECT(UndoOperation)

public:

	UndoOperation();
	virtual ~UndoOperation();

	virtual void undo() = 0;
	virtual void redo() = 0;

	bool lastUndone;
	String description;
};

TYPEDEF_INTRUSIVE_POINTER_FROM_TYPE(UndoOperation);

//-----------------------------------//

NAMESPACE_EDITOR_END