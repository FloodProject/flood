/************************************************************************
*
* vapor3D Editor © (2008-2010)
*
*	<http://www.vapor3d.org>
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