/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

#include "Document.h"

NAMESPACE_EDITOR_BEGIN

//-----------------------------------//

/**
 * Document windows are the windows that the documents display.
 */

class DocumentWindow : public wxPanel
{
public:

	DocumentWindow( wxWindow* parent, wxWindowID id = wxID_ANY, 
		const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize,
		long style = wxFULL_REPAINT_ON_RESIZE | wxBORDER_NONE );

	virtual ~DocumentWindow();

	// Sets the current document bar.
	void setDocumentBar( DocumentBar* );

	// Flags the backing control for redraw.
	virtual void flagRedraw() {}

	// Gets the sizer of this panel.
	GETTER(Sizer, wxSizer*, mainSizer)

	// Main sizer.
	wxSizer* mainSizer;
};

//-----------------------------------//

NAMESPACE_EDITOR_END