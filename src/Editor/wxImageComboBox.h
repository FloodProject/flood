/************************************************************************
*
* vapor3D Editor © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include <wx/odcombo.h>

namespace vapor { namespace editor {

//-----------------------------------//

/**
 * Implements a media browser. At the moment it only lets the user
 * select a texture from a possibly very big list, but it can be 
 * expanded for other media types later. It uses virtual item lists
 * so it can support infinitely big lists.
 */

class wxImageComboBox : public wxOwnerDrawnComboBox
{
public:

	wxImageComboBox( wxWindow *parent, wxWindowID id );
	
	virtual void OnDrawBackground (wxDC &dc, const wxRect &rect, 
		int item, int flags) const;

	virtual void OnDrawItem (wxDC &dc, const wxRect &rect, int item,
		int flags) const;
	
	virtual wxCoord	OnMeasureItem (size_t item) const;
	virtual wxCoord	OnMeasureItemWidth (size_t item) const;

	void addImage( const wxString& item, const ImagePtr& image );

protected:

	wxBitmap* convertToBitmap( const ImagePtr& image ) const;

	std::vector<ImagePtr> images;

	DECLARE_EVENT_TABLE()
};

//-----------------------------------//

} } // end namespaces