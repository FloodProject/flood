/************************************************************************
*
* vapor3D Editor © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

//---------------------------------------------------------------------//
// wxWidgets headers
//---------------------------------------------------------------------//

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"
 
// for all others, include the necessary headers (this file is usually 
// all you need because it includes almost all "standard" headers)
#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif

#include <wx/mstream.h>
#include <wx/hyperlink.h>
#include <wx/treectrl.h> 
#include <wx/choicebk.h>
#include <wx/notebook.h>
#include <wx/statline.h>
#include <wx/spinctrl.h>
#include <wx/filectrl.h>

//---------------------------------------------------------------------//
// vapor3D Engine
//---------------------------------------------------------------------//

#define VAPOR_USE_NAMESPACES
#include <vapor/Headers.h>

//---------------------------------------------------------------------//
// Image Utilities
//---------------------------------------------------------------------//

#include "ImageUtils.h"
