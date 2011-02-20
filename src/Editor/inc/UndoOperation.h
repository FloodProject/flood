/************************************************************************
*
* vapor3D Editor © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "ReferenceCount.h"

namespace vapor { namespace editor {

//-----------------------------------//

/**
 * Represents an editing operation that can be undone and redone.
 * This is the base class of the undo/redo system and every operation
 * that the engine provides should be provided using this type.
 */

class UndoOperation : public ReferenceCounted
{
public:

	UndoOperation();
	virtual ~UndoOperation();

	virtual void undo() = 0;
	virtual void redo() = 0;

	bool lastUndone;
	std::string description;
};

TYPEDEF_INTRUSIVE_POINTER_FROM_TYPE(UndoOperation);

//-----------------------------------//

} } // end namespaces