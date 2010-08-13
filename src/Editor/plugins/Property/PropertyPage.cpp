/************************************************************************
*
* vapor3D Editor © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "PCH.h"
#include "PropertyPage.h"
#include "EditorIcons.h"
#include "Editor.h"

namespace vapor { namespace editor {

BEGIN_EVENT_TABLE(PropertyPage, wxPropertyGrid)
	//EVT_TREE_ITEM_MENU(ID_SceneTree, PropertyPage::onItemMenu)
END_EVENT_TABLE()

//-----------------------------------//

PropertyPage::PropertyPage( EditorFrame* editor,
							 wxWindow* parent, wxWindowID id,
							 const wxPoint& pos, const wxSize& size )
	: wxPropertyGrid(parent, id, pos, size, wxPG_DEFAULT_STYLE | wxPG_SPLITTER_AUTO_CENTER)
	, editor(editor)
{
	engine = editor->getEngine();
	assert( engine != nullptr );

	initControl();
}

//-----------------------------------//

void PropertyPage::initControl()
{
	// Make the default font a little smaller...
	SetFont( editor->GetFont().Scaled(0.9f) );

	wxColour color = wxLIGHT_GREY->ChangeLightness(150);
	
	SetMarginColour( color );
	SetLineColour( color );
	SetCaptionBackgroundColour( color );
	SetCaptionTextColour( *wxBLACK );
}

//-----------------------------------//

PropertyPage::~PropertyPage()
{

}

//-----------------------------------//

void PropertyPage::updateTree()
{

}

//-----------------------------------//

void PropertyPage::onItemMenu(wxTreeEvent& WXUNUSED(event))
{

}

//-----------------------------------//

} } // end namespaces