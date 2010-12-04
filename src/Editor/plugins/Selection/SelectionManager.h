/************************************************************************
*
* vapor3D Editor © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "scene/Entity.h"

namespace vapor { namespace editor {

//-----------------------------------//

namespace SelectionMode
{
	enum Enum
	{
		Entity,
		Face,
		Edge,
		Vertex
	};
}

struct SelectionData
{
	EntityPtr entity;
	SelectionMode::Enum mode;
};

/**
 * Manages and keeps track of selections, which can be of different modes.
 * You can also make group selections by using a drag select operation.
 * This also makes sure that selections are treated as first-class operations
 * in the editor undo/redo system.
 */

class SelectionManager
{
public:

	SelectionManager();

	// Gets the current selection mode.
	SelectionMode::Enum getSelectionMode();

	// Sets the current selection mode.
	void setSelectionMode(SelectionMode::Enum mode);

private:

	// Selection mode.
	SelectionMode::Enum mode;
	std::vector<SelectionData*> selections;
};

//-----------------------------------//

} } // end namespaces