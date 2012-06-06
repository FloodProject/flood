/////////////////////////////////////////////////////////////////////////////
// Name:        foldpanelbar.cpp
// Purpose:
// Author:      Jorgen Bodde
// Modified by: ABX - 19/12/2004 : possibility of horizontal orientation
//                               : wxWidgets coding standards
// Created:     22/06/2004
// RCS-ID:      $Id: foldpanelbar.cpp,v 1.1 2007-03-28 15:18:13 frm Exp $
// Copyright:   (c) Jorgen Bodde
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#include "Editor/API.h"

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif

#include "Widgets/sequentialsizer.h"
#include "wx/foldbar/foldpanelbar.h"

//----------------------------------------------------------------------------
// wxFoldPanelBar
//----------------------------------------------------------------------------

IMPLEMENT_DYNAMIC_CLASS( wxFoldPanelBar, wxPanel )

BEGIN_EVENT_TABLE(wxFoldPanelBar,wxPanel)
    EVT_CAPTIONBAR(wxID_ANY, wxFoldPanelBar::OnPressCaption)
	EVT_SIZE(wxFoldPanelBar::OnSizePanel)
	//EVT_PAINT(wxFoldPanelBar::OnPaint)
	//EVT_ERASE_BACKGROUND(wxFoldPanelBar::OnEraseBackground)
END_EVENT_TABLE()

wxFoldPanelBar::wxFoldPanelBar()
    : wxScrolledWindow()
    , m_foldPanel(NULL)
    , m_bottomPanel(NULL)
    , m_panelSizer(NULL)
    , m_controlCreated(false)
{

}

wxFoldPanelBar::wxFoldPanelBar( wxWindow *parent, wxWindowID id, const wxPoint &position,
                                const wxSize& size, long style, long extraStyle)
    : wxScrolledWindow()
    , m_foldPanel(NULL)
    , m_bottomPanel(NULL)
    , m_panelSizer(NULL)
    , m_controlCreated(false)
{
    Create( parent, id, position, size, style, extraStyle);
}

void wxFoldPanelBar::Create( wxWindow *parent, wxWindowID id, const wxPoint &position,
                             const wxSize& size, long style, long extraStyle )
{

    m_extraStyle = extraStyle;

    // make sure there is any orientation
    if ( (style & wxFPB_HORIZONTAL) != wxFPB_HORIZONTAL )
        style |= wxFPB_VERTICAL;

    // create the panel (duh!). This causes a size event, which we are going
    // to skip when we are not initialised

    wxPanel::Create(parent, id, position, size, style);

	// Erase the background.
	SetBackgroundStyle(wxBG_STYLE_SYSTEM);

    // the fold panel area
    m_panelSizer = new /*wxBoxSizer*/wxSequentialSizer(wxVERTICAL);

    SetSizer(m_panelSizer);

	FitInside(); // ask the sizer about the needed size
	SetScrollRate(5, 5);

#if 0
    // create the fold icons to be used in the captions
    m_images = new wxImageList(16, 16);

    wxBitmap *bmp = new wxBitmap(icon_expanded);
    m_images->Add(*bmp);
    delete bmp;

    bmp = new wxBitmap(icon_collapsed);
    m_images->Add(*bmp);
    delete bmp;

    m_moreBmp = new wxBitmap(icon_theresmore);
#else
    m_images = NULL;
    m_moreBmp = NULL;
#endif

    // do this as last, to check if create is already called

    m_controlCreated = true;
}

wxFoldPanelBar::~wxFoldPanelBar()
{
    delete m_images;
    delete m_moreBmp;
}

bool wxFoldPanelBar::Layout()
{
	return ScrollLayout();
}

void wxFoldPanelBar::OnSizePanel(wxSizeEvent &event)
{
	FitInside();
}


wxFoldPanel wxFoldPanelBar::AddFoldPanel(const wxString &caption, int proportion,
	bool collapsedInitially, const wxCaptionBarStyle &style)
{
    wxASSERT(m_controlCreated);

    // create a fold panel item, which is first only the caption.
    // the user can now add a panel area which will be folded in
    // when pressed.

    wxFoldPanelItem *item = new wxFoldPanelItem(this, caption, m_images, collapsedInitially, style);

	//item->SetWindowStyle(wxBORDER_STATIC);

	m_panelSizer->Add(item, wxSizerFlags().Expand().Proportion(proportion));
    m_panels.Add(item);

    return wxFoldPanel(item);
}

int wxFoldPanelBar::AddFoldPanelWindow(const wxFoldPanel &panel, wxWindow *window, wxSizerFlags sizerFlags,
    int flags, int ySpacing, int leftSpacing, int rightSpacing)
{
    wxCHECK(panel.IsOk(), -1);
    panel.GetItem()->AddWindow(window, sizerFlags, flags, ySpacing, leftSpacing, rightSpacing);

    return 0;
}

void wxFoldPanelBar::OnPressCaption(wxCaptionBarEvent &event)
{
    // act upon the folding or expanding status of the bar

    if(event.GetFoldStatus())
        Collapse(wxFoldPanel((wxFoldPanelItem *)event.GetTag()));
    else
        Expand(wxFoldPanel((wxFoldPanelItem *)event.GetTag()));
}

