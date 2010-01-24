/************************************************************************
*
* vaporEngine (2008-2010)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#pragma once

#include "wx/mstream.h"
#include <wx/artprov.h>

//-----------------------------------//

#define wxMEMORY_IMAGE( name ) _wxConvertMemoryToImage( name, sizeof( name ) )
#define wxMEMORY_IMAGEEX( name, type ) _wxConvertMemoryToImage( name, sizeof( name ), type )
#define wxMEMORY_BITMAP( name ) _wxConvertMemoryToBitmap( name, sizeof( name ) )
#define wxMEMORY_BITMAPEX( name, type ) _wxConvertMemoryToBitmap( name, sizeof( name ), type )

//-----------------------------------//

inline wxImage _wxConvertMemoryToImage(const unsigned char* data, int length, long type = wxBITMAP_TYPE_ANY )
{
	wxMemoryInputStream stream( data, length );
	return wxImage( stream );
}

inline wxBitmap _wxConvertMemoryToBitmap(const unsigned char* data, int length, long type = wxBITMAP_TYPE_ANY )
{
	wxMemoryInputStream stream( data, length );
	return wxBitmap( wxImage( stream ), -1 );
}

//-----------------------------------//

class ArtProvider : public wxArtProvider
{
protected:

	wxBitmap CreateBitmap(const wxArtID& id,
						  const wxArtClient& client,
						  const wxSize& size);

	// optionally override this one as well
	//wxIconBundle CreateIconBundle(const wxArtID& id,
								  //const wxArtClient& client)
	//{ }
};

//-----------------------------------//