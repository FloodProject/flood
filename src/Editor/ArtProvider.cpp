/************************************************************************
*
* vaporEngine (2008-2009)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#include "ArtProvider.h"
#include "toolbar_icons.h"

//-----------------------------------//

wxBitmap ArtProvider::CreateBitmap(const wxArtID& id,
                                     const wxArtClient& client,
                                     const wxSize& WXUNUSED(size))
{
    if ( client == wxART_TOOLBAR )
    {
		if(id == wxART_DELETE)
			return wxMEMORY_BITMAP(disk);
    }
    return wxNullBitmap;
}

//-----------------------------------//