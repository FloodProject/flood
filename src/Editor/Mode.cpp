/************************************************************************
*
* vapor3D Editor © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "PCH.h"
#include "Mode.h"
#include "Editor.h"

namespace vapor { namespace editor {

//-----------------------------------//

Mode::Mode( EditorFrame* frame )
	: editor( frame )
{
	assert( editor != nullptr );

	engine = editor->engine;
	viewport = editor->viewport;
}

//-----------------------------------//

Mode::~Mode()
{

}

//-----------------------------------//

} } // end namespaces