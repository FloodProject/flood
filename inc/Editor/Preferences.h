/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

#include <wx/propdlg.h>
#include "GUI/PreferencesGenerated.h"

NAMESPACE_EDITOR_BEGIN

//-----------------------------------//

class EditorFrame;

class Preferences : public wxPropertySheetDialog
{
public:

	Preferences( wxWindow* parent );
	bool createPages();

protected:

	Bindings* bindings;
	Plugins* plugins;
	Resources* resources;
	Renderers* renderers;

	wxListCtrl* m_listPlugins;
};

//-----------------------------------//

NAMESPACE_EDITOR_END