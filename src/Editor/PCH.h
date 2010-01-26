/************************************************************************
*
* vaporEngine (2008-2010)
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

#define VAPOR_USE_NAMESPACES
#include <vapor/Headers.h>

#include <wx/glcanvas.h>
#include <wx/hyperlink.h>
#include <wx/treectrl.h> 
#include <wx/choicebk.h>
#include <wx/notebook.h>
#include <wx/statline.h>

#include "ArtProvider.h"

#include <boost/lexical_cast.hpp>
