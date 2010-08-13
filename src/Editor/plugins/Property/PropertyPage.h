/************************************************************************
*
* vapor3D Editor © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include <wx/propgrid/propgrid.h>

namespace vapor { namespace editor {

//-----------------------------------//

class EditorFrame;

//-----------------------------------//

/**
 * This control is responsible for providing a page with component and
 * node properties, so they can be viewed and edited.
 */

class PropertyPage : public wxPropertyGrid
{
public:

	PropertyPage( EditorFrame* editor,
		wxWindow* parent, wxWindowID id = wxID_ANY,
		const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize );

	~PropertyPage();

	// Synchronizes the tree with the resources.
	void updateTree();

protected:

	void initControl();

	void onItemMenu(wxTreeEvent& event);

	EditorFrame* editor;
	Engine* engine;

private:

	DECLARE_EVENT_TABLE()
};

//-----------------------------------//

} } // end namespaces