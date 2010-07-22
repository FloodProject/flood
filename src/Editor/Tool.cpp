/************************************************************************
*
* vapor3D Editor © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "PCH.h"
#include "Tool.h"
#include "Editor.h"

namespace vapor { namespace editor {

//-----------------------------------//

Tool::Tool( EditorFrame* frame )
	: editor( frame )
{
	assert( editor != nullptr );

	engine = editor->engine;
	viewframe = editor->viewframe;
}

//-----------------------------------//

Tool::~Tool()
{

}

//-----------------------------------//

} } // end namespaces