/************************************************************************
*
* vapor3D Editor © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "PCH.h"
#include "wxImageComboBox.h"

namespace vapor { namespace editor {

////////////////////////////////////////////////////////////
// Event table
////////////////////////////////////////////////////////////

BEGIN_EVENT_TABLE(wxImageComboBox, wxOwnerDrawnComboBox)
END_EVENT_TABLE()

//-----------------------------------//

wxImageComboBox::wxImageComboBox( wxWindow* parent, wxWindowID id )
	: wxOwnerDrawnComboBox( parent, id, wxEmptyString, wxDefaultPosition, 
	wxSize(-1, 128), 0, nullptr, wxCC_STD_BUTTON/*|wxCB_READONLY*/ )
{
	SetCustomPaintWidth( 150 );
	GetTextCtrl()->Disable();
}

//-----------------------------------//

void wxImageComboBox::addImage( const wxString& item, const ImagePtr& image )
{
	assert( image != nullptr );

	Append( wxEmptyString );
	images.push_back(image);
}

//-----------------------------------//

wxBitmap* wxImageComboBox::convertToBitmap( const ImagePtr& image ) const
{
	if( !image )
		return nullptr;

	assert( image->getPixelFormat() == PixelFormat::R8G8B8 );
	
	byte* buf = const_cast<byte*>(&image->getBuffer()[0]);
	wxImage img( image->getWidth(), image->getHeight(), buf, true /* don't free data */);

	const wxSize& size = GetSize();
	if( img.GetWidth() > size.GetWidth() || img.GetHeight() > size.GetHeight() )
	{
		img.Rescale( size.GetWidth(), size.GetHeight() );
	}

	if( !img.HasAlpha() )
	{
		img.SetMaskColour( 255, 255, 255 );
		img.InitAlpha();
	}

	wxBitmap* bmp = new wxBitmap( img );
	return bmp;
}

//-----------------------------------//

void wxImageComboBox::OnDrawBackground (wxDC &dc, const wxRect &rect,
										int item, int flags) const
{
	if ( item == wxNOT_FOUND )
		return;

	const ImagePtr& image = images[item];
	wxBitmap* bmp = convertToBitmap( image );

	dc.DrawBitmap( *bmp, rect.GetTopLeft() );
	
	if( flags & wxODCB_PAINTING_SELECTED )
		dc.DrawRectangle( rect );

	delete bmp;
}

//-----------------------------------//

void wxImageComboBox::OnDrawItem (wxDC &dc, const wxRect &rect,
								  int item, int flags) const
{
	if ( item == wxNOT_FOUND )
		return;

	const ImagePtr& image = images[item];
	wxBitmap* bmp = convertToBitmap( image );

	dc.DrawBitmap( *bmp, rect.GetTopLeft() );

	delete bmp;
}

//-----------------------------------//

wxCoord	wxImageComboBox::OnMeasureItem (size_t item) const
{
	const ImagePtr& image = images[item];
	return image->getHeight();
}

//-----------------------------------//

wxCoord	wxImageComboBox::OnMeasureItemWidth (size_t item) const
{
	const ImagePtr& image = images[item];
	return image->getWidth();
}

//-----------------------------------//

} } // end namespaces