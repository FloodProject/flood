/************************************************************************
*
* vapor3D Editor © (2008-2010)
*
*	<http://www.vapor3d.org>
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