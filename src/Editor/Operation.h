/************************************************************************
*
* vapor3D Editor © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

namespace vapor { namespace editor {

//-----------------------------------//

/**
 * Represents an editing operation that can be undone and redone.
 * This is the base class of the undo/redo system and every operation
 * that the engine provides should be provided using this type.
 */

class Operation
{
public:

	Operation();
	virtual ~Operation();

	virtual void do_() = 0;
	virtual void undo() = 0;

protected:

};

//-----------------------------------//

} } // end namespaces