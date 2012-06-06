
/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Editor/API.h"

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#include "Widgets/sequentialsizer.h"

wxIMPLEMENT_CLASS(wxSequentialSizer, wxBoxSizer)

void wxSequentialSizer::RecalcSizes()
{
    if ( m_children.empty() )
        return;

    const wxCoord totalMinorSize = GetSizeInMinorDir(m_size);

	wxSizerItemList::const_iterator i;  // iterator in m_children list

	wxPoint pos;

    for ( i = m_children.begin(); i != m_children.end(); ++i )
    {
        wxSizerItem * const item = *i;

        if ( !item->IsShown() )
            continue;

        wxSize szMinPrev = item->GetMinSizeWithBorder();
        item->InformFirstDirection(m_orient^wxBOTH,totalMinorSize);
        wxSize szMin = item->GetMinSizeWithBorder();

		wxCoord minorSize = GetSizeInMinorDir(szMin);
		wxCoord majorSize = GetSizeInMajorDir(szMin);

		if ( (item->GetFlag() & (wxEXPAND | wxSHAPED)) || (minorSize > totalMinorSize) )
        {
            // occupy all the available space if wxEXPAND was given
            minorSize = totalMinorSize;
		}


		item->SetDimension(pos, SizeFromMajorMinor(majorSize, minorSize));

		PosInMajorDir(pos) += majorSize;
    }
}