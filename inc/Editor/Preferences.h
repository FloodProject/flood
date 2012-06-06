/************************************************************************
*
* vapor3D Editor © (2008-2010)
*
*	<http://www.vapor3d.org>
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