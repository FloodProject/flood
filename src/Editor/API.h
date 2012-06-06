/************************************************************************
*
* vapor3D Editor © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#define NOMINMAX

#include "Core/API.h"
#include "Engine/API.h"
#include "Pipeline/API.h"
#include "Engine/Headers.h"

#define VAPOR_EDITOR_NAME "Flush3D"

#define wxUSE_BINDERAPP

#define EDITOR_OLD_UI

//---------------------------------------------------------------------//
// API Exports
//---------------------------------------------------------------------//

#if defined(API_EDITOR_DLL) && defined(API_EDITOR_DLL_EXPORT)
	#define API_EDITOR API_EXPORT
#elif defined(API_EDITOR_DLL)
	#define API_EDITOR API_IMPORT
#else
	#define API_EDITOR
#endif

//---------------------------------------------------------------------//
// API Namespaces
//---------------------------------------------------------------------//

#define NAMESPACE_EDITOR_BEGIN 
#define NAMESPACE_EDITOR_END 

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

#include "Editor/ImageUtils.h"

NAMESPACE_EDITOR_BEGIN
typedef CameraPtr CameraWeakPtr;
typedef ScenePtr SceneWeakPtr;
typedef ComponentPtr ComponentWeakPtr;
typedef EntityPtr EntityWeakPtr;
NAMESPACE_EDITOR_END