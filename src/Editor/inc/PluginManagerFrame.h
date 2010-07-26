/************************************************************************
*
* vapor3D Editor © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include <wx/checklst.h>

namespace vapor { namespace editor {

class PluginManager;

//-----------------------------------//

/**
 * Provides functionality to list known plugins. Each plugin shows its
 * metadata and can be enabled and disabled on the fly.
 */

class PluginManagerFrame : public wxFrame 
{
public:

	PluginManagerFrame( wxWindow* parent, PluginManager* );

protected:

	// Initializes the control.
	void InitControl();

	// Event handlers.
	void OnClose(wxCloseEvent& event);
	void OnCheckEvent(wxCommandEvent& event);

	// Manages all the plugins.
	PluginManager* pluginManager;

	// Widget where plugin metadata is shown.
	wxCheckListBox *checkListBox;

	DECLARE_EVENT_TABLE()
};

//-----------------------------------//

} } // end namespaces