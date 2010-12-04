/************************************************************************
*
* vapor3D Editor © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "PCH.h"
#include "SelectionManager.h"

namespace vapor { namespace editor {

//-----------------------------------//

SelectionManager::SelectionManager()
	: mode(SelectionMode::Vertex)
{ }

//-----------------------------------//

SelectionMode::Enum SelectionManager::getSelectionMode()
{
	return mode;
}

//-----------------------------------//

void SelectionManager::setSelectionMode(SelectionMode::Enum mode)
{
	this->mode = mode;
}

//-----------------------------------//

} } // end namespaces