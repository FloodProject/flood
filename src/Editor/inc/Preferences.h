/************************************************************************
*
* vapor3D Editor © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include <wx/propdlg.h>
#include "gen/PreferencesGenerated.h"

namespace vapor { namespace editor {

//-----------------------------------//

class EditorFrame;

class Preferences : public wxPropertySheetDialog
{
public:

	Preferences( wxWindow* parent );
	
	bool createPages();
	void setupPlugins();
	
	void OnPluginSelected( wxListEvent& event );
	void OnPluginEnable( wxCommandEvent& event );
	void OnPluginCheckUpdates( wxCommandEvent& event );
	void OnPluginInstall( wxCommandEvent& event );

protected:

	gui::Bindings* bindings;
	gui::Plugins* plugins;
	gui::Resources* resources;

	wxListCtrl* m_listPlugins;
};

//-----------------------------------//

} } // end namespaces