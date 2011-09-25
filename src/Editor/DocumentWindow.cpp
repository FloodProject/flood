/************************************************************************
*
* vapor3D Editor © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Editor/API.h"
#include "DocumentWindow.h"

NAMESPACE_EDITOR_BEGIN

//-----------------------------------//

DocumentWindow::DocumentWindow( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style ) 
	: wxPanel(parent, id, pos, size, style | wxBORDER_NONE | wxFULL_REPAINT_ON_RESIZE)
{
	mainSizer = new wxBoxSizer( wxVERTICAL );
	SetSizer( mainSizer );
}

//-----------------------------------//

DocumentWindow::~DocumentWindow()
{
	LogDebug("Destroying DocumentWindow");
}

//-----------------------------------//

void DocumentWindow::setDocumentBar( DocumentBar* documentBar )
{

}

//-----------------------------------//

NAMESPACE_EDITOR_END
