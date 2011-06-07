/************************************************************************
*
* vapor3D Editor © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "Core/API.h"
#include "Engine/API.h"
#include "Pipeline/API.h"
#include "Headers.h"

#define VAPOR_EDITOR_NAME "vapor3D"

//#define ENABLE_PLUGIN_MONO
#define ENABLE_PLUGIN_TERRAIN
#define ENABLE_PLUGIN_GIZMO
#define ENABLE_PLUGIN_PROPERTY

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

#include <wx/event.h> 
#include <wx/mstream.h>
#include <wx/hyperlink.h>
#include <wx/treectrl.h> 
#include <wx/choicebk.h>
#include <wx/notebook.h>
#include <wx/statline.h>
#include <wx/spinctrl.h>
#include <wx/filectrl.h>
#include <wx/splitter.h>
#include <wx/progdlg.h>
#include <wx/dnd.h>
#include <wx/aui/framemanager.h>
#include <wx/aui/auibook.h>
#include <wx/aui/auibar.h>
#include <wx/propgrid/propgrid.h>
#include <wx/propgrid/advprops.h>

//---------------------------------------------------------------------//
// Image Conversion Utilities
//---------------------------------------------------------------------//

#include "ImageUtils.h"