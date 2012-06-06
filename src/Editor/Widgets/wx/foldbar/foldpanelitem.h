/////////////////////////////////////////////////////////////////////////////
// Name:        foldpanelitem.h
// Purpose:     wxFoldPanel
// Author:      Jorgen Bodde
// Modified by: ABX - 19/12/2004 : possibility of horizontal orientation
//                               : wxWidgets coding standards
// Created:     22/06/2004
// RCS-ID:      $Id: foldpanelitem.h,v 1.7 2005/07/28 23:23:22 VZ Exp $
// Copyright:   (c) Jorgen Bodde
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef __WXFOLDPANELITEM_H__
#define __WXFOLDPANELITEM_H__

#include "wx/foldbar/captionbar.h"

#define wxFPB_ALIGN_LEFT  0
#define wxFPB_ALIGN_WIDTH 1

#define wxFPB_DEFAULT_LEFTSPACING  5
#define wxFPB_DEFAULT_RIGHTSPACING 10
#define wxFPB_DEFAULT_SPACING     8

#define wxFPB_DEFAULT_LEFTLINESPACING  2
#define wxFPB_DEFAULT_RIGHTLINESPACING 2

class wxFoldWindowItem
{
private:
    wxWindow *m_wndItem;
    int m_type, m_flags;
    int m_leftSpacing,
        m_rightSpacing,
        m_Spacing;
    int m_lineLength, m_lineY;
    wxColour m_sepLineColour;

public:
    enum
    {
        WINDOW = 0,
        SEPARATOR
    };

    // wxWindow constructor. This initialises the class as a wxWindow type
    wxFoldWindowItem(wxWindow *wnd, int flags = wxFPB_ALIGN_WIDTH, int Spacing = wxFPB_DEFAULT_SPACING,
                     int leftSpacing = wxFPB_DEFAULT_LEFTSPACING, int rightSpacing = wxFPB_DEFAULT_RIGHTSPACING)
        : m_wndItem(wnd)
        , m_type(WINDOW)
        , m_flags(flags)
        , m_leftSpacing(leftSpacing)
        , m_rightSpacing(rightSpacing)
        , m_Spacing(Spacing)
        , m_lineLength(0)
        , m_lineY(0)
    {
    };

    // separator constructor. This initialises the class as a separator type
    wxFoldWindowItem(int y, const wxColour &lineColor = *wxBLACK, int Spacing = wxFPB_DEFAULT_SPACING,
                     int leftSpacing = wxFPB_DEFAULT_LEFTLINESPACING,
                     int rightSpacing = wxFPB_DEFAULT_RIGHTLINESPACING)
        : m_wndItem(NULL)
        , m_type(SEPARATOR)
        , m_flags(wxFPB_ALIGN_WIDTH)
        , m_leftSpacing(leftSpacing)
        , m_rightSpacing(rightSpacing)
        , m_Spacing(Spacing)
        , m_lineLength(0)
        , m_lineY(y)
        , m_sepLineColour(lineColor)
    {
    };

    // TODO: Make a c'tor for a captioned splitter

    int GetType() const {
        return m_type;
    };

    int GetLineY() const {
        return m_lineY;
    };

    int GetLineLength() const {
        return m_lineLength;
    };

    const wxColour &GetLineColour() const {
        return m_sepLineColour;
    };

    int GetLeftSpacing() const {
        return m_leftSpacing;
    };

    int GetRightSpacing() const {
        return m_rightSpacing;
    };

    int GetSpacing() const {
        return m_Spacing;
    };
};

#include <wx/dynarray.h>
WX_DECLARE_OBJARRAY_WITH_DECL(wxFoldWindowItem, wxFoldWindowItemArray, class WXDLLIMPEXP_FOLDBAR);

#ifndef _NO_DOXYGEN_

/** \wxFoldPanelItem
    This class is a child sibling of the wxFoldPanelBar class. It will be containing a wxCaptionBar class
    for receiving of events, and a the rest of the area can be populated by a wxPanel derived class.
*/

class WXDLLIMPEXP_FOLDBAR wxFoldPanelItem: public wxPanel
{
private:
    wxSizer* m_sizer;
    wxCaptionBar *m_captionBar;

    bool m_controlCreated;
    int m_userSize,
        m_panelSize,
        m_lastInsertPos;
    int m_itemPos;
    bool m_userSized;

private:
    DECLARE_CLASS( wxFoldPanelItem )
    DECLARE_EVENT_TABLE()

private:
    wxFoldWindowItemArray m_items;

    void OnPressCaption(wxCaptionBarEvent &event);
    void OnPaint(wxPaintEvent &event);

public:
    // constructors and destructors
    wxFoldPanelItem( wxWindow *parent, const wxString &caption, wxImageList *icons = 0, bool collapsedInitially = false,
                     const wxCaptionBarStyle &style = wxEmptyCaptionBarStyle);
    virtual ~wxFoldPanelItem();

	//wxSize DoGetBestSize() const OVERRIDE;

    /** Add a window item to the list of items on this panel. The flags are wxFPB_ALIGN_LEFT for a non sizing
        window element, and wxFPB_ALIGN_WIDTH for a width aligned item. The ySpacing parameter reserves a number
        of pixels before the window element, and leftSpacing is an indent. rightSpacing is only relevant when the
        style wxFPB_ALIGN_WIDTH is chosen. */
    void AddWindow(wxWindow *window, wxSizerFlags sizerFlags, int flags, int ySpacing, int leftSpacing, int rightSpacing);

    /** Return expanded or collapsed status. If the panel is expanded, true is returned */
    bool IsExpanded() const {
        return !m_captionBar->IsCollapsed();
    };

    // this should not be called by the user, because it doesn't trigger the parent
    // to tell it that we are collapsed or expanded, it only changes visual state
    void Collapse();

    // this should not be called by the user, because it doesn't trigger the parent
    // to tell it that we are collapsed or expanded, it only changes visual state
    void Expand();

    bool IsVertical() const;

    // returns space of caption only. This is for folding calulation
    // purposes

    int GetCaptionLength() const {
        wxSize size = m_captionBar->GetSize();
        return IsVertical() ? size.GetHeight() : size.GetWidth();
    };

    void ApplyCaptionStyle(const wxCaptionBarStyle &style) {
        wxCHECK2(m_captionBar, return);
        m_captionBar->SetCaptionStyle(false, style);
    };

    wxCaptionBarStyle GetCaptionStyle() {
        wxCHECK(m_captionBar, wxEmptyCaptionBarStyle);
        return m_captionBar->GetCaptionStyle();
    };

    /** Returns if the caption is visible. */
    wxCaptionBar* GetCaptionBar() const {
        return m_captionBar;
    };

	void OnSize(wxSizeEvent&);
};


#endif // _NO_DOXYGEN_

#endif // __WXFOLDPANELITEM_H__
