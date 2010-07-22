/************************************************************************
*
* vapor3D Editor © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

namespace vapor { namespace editor {

class Tool;

//-----------------------------------//

/**
 * Responsible for managing tools in the editor.
 */

class ToolManager
{
public:

	ToolManager();

protected:

	std::vector<Tool*> tools;
};

//-----------------------------------//

} } // end namespaces