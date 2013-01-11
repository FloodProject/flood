/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

#include "wx/propgrid/propgrid.h"

#ifdef ENABLE_PLUGIN_PROPERTY

NAMESPACE_EDITOR_BEGIN

//-----------------------------------//

class PropertyGrid : public wxPropertyGrid
{
public:

	PropertyGrid(wxWindow* parent);

	wxSize DoGetBestSize() const OVERRIDE;
	void OnIdle(wxIdleEvent&);

	mutable wxCoord virtualHeight;
};

//-----------------------------------//

NAMESPACE_EDITOR_END

#endif