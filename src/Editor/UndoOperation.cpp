/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#include "Editor/API.h"
#include "UndoOperation.h"

NAMESPACE_EDITOR_BEGIN

//-----------------------------------//

REFLECT_ABSTRACT_CHILD_CLASS(UndoOperation, Object)
REFLECT_CLASS_END()

//-----------------------------------//

UndoOperation::UndoOperation()
	: lastUndone(false)
{ }

//-----------------------------------//

UndoOperation::~UndoOperation()
{ }

//-----------------------------------//

NAMESPACE_EDITOR_END