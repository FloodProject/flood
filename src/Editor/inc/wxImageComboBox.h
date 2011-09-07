/************************************************************************
*
* vapor3D Editor © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include <wx/odcombo.h>
#include "Resources/Image.h"

NAMESPACE_EDITOR_BEGIN

//-----------------------------------//

wxBitmap* ConvertImageToBitmap( Image* image, const Path& fullPath );

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
		int item, int flags) const OVERRIDE;

	virtual void OnDrawItem (wxDC &dc, const wxRect &rect, int item,
		int flags) const OVERRIDE;
	
	virtual wxCoord	OnMeasureItem (size_t item) const OVERRIDE;
	virtual wxCoord	OnMeasureItemWidth (size_t item) const OVERRIDE;

	void addImage( const ImageHandle& image );

protected:

	std::vector<ImageHandle> images;
};

//-----------------------------------//

NAMESPACE_EDITOR_END