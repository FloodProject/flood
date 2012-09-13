/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

#include "wx/sizer.h"
#include "wx/window.h"     

/** Implementation of a sequential sizer based on wxBoxSizer for the size logic.
* Basically the sequential sizer doesn't try to divide the space between the items
* but just allocates enough space to cover all the items at their best sizes.
* This is meant to be used with scrollbars so each widget can be fully sized.
*/

class /*WXDLLIMPEXP_CORE*/ wxSequentialSizer : public wxBoxSizer
{
public:

    wxSequentialSizer(int orient) : wxBoxSizer(orient)
    {
    }

    virtual void RecalcSizes() OVERRIDE;

private:
    DECLARE_CLASS(wxSequentialSizer)
};