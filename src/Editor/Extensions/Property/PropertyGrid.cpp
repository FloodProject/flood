/************************************************************************
*
* vapor3D Editor © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Editor/API.h"
#include "PropertyGrid.h"
#include "Editor.h"

#ifdef ENABLE_PLUGIN_PROPERTY

NAMESPACE_EDITOR_BEGIN

//-----------------------------------//

PropertyGrid::PropertyGrid(wxWindow* parent)
	: wxPropertyGrid(parent, wxID_ANY, wxDefaultPosition,
	wxDefaultSize, wxPG_DEFAULT_STYLE | wxPG_SPLITTER_AUTO_CENTER
	| wxPG_NO_INTERNAL_BORDER  )
{
	// Make the default font a little smaller.
	SetFont( GetEditor().GetFont().Scaled(0.9f) );

	// Switch to slighty lighter colors.
	wxColour color = wxLIGHT_GREY->ChangeLightness(120);
	SetLineColour( color );
	SetCaptionBackgroundColour( color );
	SetCaptionTextColour( *wxBLACK );

	color = wxLIGHT_GREY->ChangeLightness(180);
	SetMarginColour(color);

	Bind(wxEVT_IDLE, &PropertyGrid::OnIdle, this);
}

//-----------------------------------//

wxSize PropertyGrid::DoGetBestSize() const
{
	virtualHeight = GetState()->GetVirtualHeight();

	wxSize best(wxDefaultCoord, virtualHeight);
	return best;
}

//-----------------------------------//

void PropertyGrid::OnIdle(wxIdleEvent&)
{
	wxCoord newHeight = GetState()->GetVirtualHeight();

	if( newHeight != virtualHeight )
	{
		// Force a re-layout of the parent widget.
		GetParent()->Fit();
		virtualHeight = newHeight;
	}
}

//-----------------------------------//

NAMESPACE_EDITOR_END

#endif