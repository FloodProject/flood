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

class PluginManagerFrame : public wxCheckListBox
{
public:

	PluginManagerFrame( wxWindow* parent, PluginManager* plugins );

protected:

	// Initializes the control.
	void InitControl();

	// Event handlers.
	void OnCheckEvent(wxCommandEvent& event);

	// Manages the plugins.
	PluginManager* plugins;
};

//-----------------------------------//

} } // end namespaces