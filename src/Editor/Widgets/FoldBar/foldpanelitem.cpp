/////////////////////////////////////////////////////////////////////////////
// Name:        foldpanelitem.cpp
// Purpose:
// Author:      Jorgen Bodde
// Modified by: ABX - 19/12/2004 : possibility of horizontal orientation
//                               : wxWidgets coding standards
// Created:     22/06/2004
// RCS-ID:      $Id: foldpanelitem.cpp,v 1.1 2007-03-28 15:18:13 frm Exp $
// Copyright:   (c) Jorgen Bodde
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#include "Editor/API.h"

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif

#include "wx/foldbar/foldpanelbar.h"
#include "wx/sequentialsizer.h"

#include <wx/arrimpl.cpp>
WX_DEFINE_OBJARRAY(wxFoldWindowItemArray);

//----------------------------------------------------------------------------
// wxFoldPanelItem
//----------------------------------------------------------------------------

IMPLEMENT_CLASS( wxFoldPanelItem, wxPanel )

BEGIN_EVENT_TABLE(wxFoldPanelItem,wxPanel)
    EVT_CAPTIONBAR(wxID_ANY, wxFoldPanelItem::OnPressCaption)
    EVT_PAINT(wxFoldPanelItem::OnPaint)
    EVT_SIZE(wxFoldPanelItem::OnSize)
END_EVENT_TABLE()

wxFoldPanelItem::wxFoldPanelItem( wxWindow *parent, const wxString &caption, wxImageList *icons, bool collapsedInitially,
                                  const wxCaptionBarStyle &style )
    : m_controlCreated(false)
    , m_sizer(nullptr)
    , m_userSize(0)
    , m_panelSize(0)
    , m_lastInsertPos(0)
    , m_itemPos(0)
    , m_userSized(false)
{
    wxCHECK2(parent, return);

    wxPanel::Create(parent, wxID_ANY);

	m_sizer = new wxSequentialSizer(IsVertical() ? wxVERTICAL : wxHORIZONTAL);
	SetSizer(m_sizer);

    // create the caption bar, in collapsed or expanded state
    m_captionBar = new wxCaptionBar(this, caption, icons, wxID_ANY, style, wxPoint(0,0));
	m_sizer->Add(m_captionBar, wxSizerFlags().Expand());
	//m_captionBar->SetBoldFont();

    if(collapsedInitially)
        m_captionBar->Collapse();

    m_controlCreated = true;

    // make initial size for component, if collapsed, the
    // size is determined on the panel height and won't change

    //wxSize size = m_captionBar->GetSize();
    //m_panelSize = IsVertical()?size.GetHeight():size.GetWidth();
    m_lastInsertPos = m_panelSize;
}

void wxFoldPanelItem::OnSize(wxSizeEvent& event)
{
	event.Skip();
}

void wxFoldPanelItem::AddWindow(wxWindow *window, wxSizerFlags sizerFlags, int flags, int ySpacing, int leftSpacing, int rightSpacing)
{
    wxASSERT(window);

    wxFoldWindowItem *wi = new wxFoldWindowItem(window, flags, ySpacing, leftSpacing, rightSpacing);
    m_items.Add(wi);

    m_sizer->Add(window, sizerFlags);

	Fit();
}


wxFoldPanelItem::~wxFoldPanelItem()
{
	m_items.Clear();
}

void wxFoldPanelItem::Collapse()
{
    m_captionBar->Collapse();
    for(size_t i = 1; i < m_sizer->GetItemCount(); i++)
        m_sizer->Hide(i);
	
	Fit();
}

void wxFoldPanelItem::Expand()
{
    m_captionBar->Expand();
    for(size_t i = 1; i < m_sizer->GetItemCount(); i++)
        m_sizer->Show(i, true);

	Fit();
}

void wxFoldPanelItem::OnPressCaption(wxCaptionBarEvent &event)
{
    // tell the upper container we are responsible
    // for this event, so it can fold the panel item
    // and do a refresh

    event.SetTag((void *)this);
    event.Skip();
}

void wxFoldPanelItem::OnPaint(wxPaintEvent& WXUNUSED(event))
{
    // draw all the items that are lines
    wxPaintDC dc(this);
    bool vertical = IsVertical();

    for(size_t i = 0; i < m_items.GetCount(); i++)
    {
        wxFoldWindowItem &item = m_items.Item(i);
        wxPen pen(item.GetLineColour(), 1, wxSOLID);
        if(item.GetType() == wxFoldWindowItem::SEPARATOR)
        {
            dc.SetPen(pen);
            int a = item.GetLeftSpacing();
            int b = item.GetLineY() + item.GetSpacing();
            int c = item.GetLineLength();
            int d = a + c;
            if (vertical)
                dc.DrawLine(a, b, d, b);
            else
                dc.DrawLine(b, a, b, d);
        }
    }
}

bool wxFoldPanelItem::IsVertical() const
{
    // grandparent of wxFoldPanelItem is wxFoldPanelBar
    // default is vertical
#if 0
    wxPanel *panel = wxDynamicCast(GetParent(), wxPanel);
    wxCHECK_MSG( panel, true, _T("wrong parent") );
#endif
    wxFoldPanelBar *bar = wxDynamicCast(GetParent(), wxFoldPanelBar);
    wxCHECK_MSG( bar, true, _T("wrong parent") );
    return bar->IsVertical();
}
