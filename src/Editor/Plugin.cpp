/************************************************************************
*
* vapor3D Editor © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "PCH.h"
#include "Plugin.h"
#include "Editor.h"

namespace vapor { namespace editor {

//-----------------------------------//

Plugin::Plugin( EditorFrame* frame )
	: editor( frame )
	, pluginEnabled(false)
{
	assert( editor != nullptr );

	engine = editor->getEngine();
	viewframe = editor->getMainViewframe();
}

//-----------------------------------//

Plugin::~Plugin()
{

}

//-----------------------------------//

bool Plugin::isPluginEnabled() const
{
	return pluginEnabled;
}

//-----------------------------------//

} } // end namespaces