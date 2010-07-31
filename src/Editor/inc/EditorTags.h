/************************************************************************
*
* vapor3D Editor © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

namespace vapor { namespace editor {

// ----------------------------------------------------------------------
// constants
// ----------------------------------------------------------------------

// IDs for the controls and the menu commands
enum
{
    // menu items
    Editor_Quit = wxID_EXIT,

    // it is important for the id corresponding to the "About" command to
	// have this standard value as otherwise it won't be handled properly
	// under Mac(where it is special and put into the "Apple" menu)
    Editor_About = wxID_ABOUT,

	// Toolbar buttons
	Toolbar_Save = 12879,
	Toolbar_ToggleScene,
	Toolbar_ToogleConsole,
	Toolbar_ToogleGrid,
	Toolbar_TooglePlay,
	Toolbar_ToogleSidebar,
	Toolbar_TooglePlugin,
	Toolbar_ToogleViewport,

	Toolbar_Undo,
	Toolbar_Redo
};

//-----------------------------------//

namespace EditorTags
{
	enum Bitfield
	{
		EditorOnly = 31,
	};
}

//-----------------------------------//

} } // end namespaces