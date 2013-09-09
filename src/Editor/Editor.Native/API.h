/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

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

#define wxID_INVALID -1