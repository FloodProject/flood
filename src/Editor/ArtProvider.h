/************************************************************************
*
* vaporEngine (2008-2009)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#pragma once
 
// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"
 
// for all others, include the necessary headers (this file is usually all you
// need because it includes almost all "standard" wxWidgets headers)
#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif

#include <wx/artprov.h> 

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