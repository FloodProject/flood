/************************************************************************
*
* vapor3D Editor © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "PCH.h"
#include "Viewframe.h"
#include "RenderWindow.h"
#include "RenderControl.h"

namespace vapor { namespace editor {

//-----------------------------------//

Viewframe::Viewframe( wxWindow* parent, wxWindowID id,
					 const wxPoint& pos, const wxSize& size, long style ) 
	: wxPanel(parent, id, pos, size, style)
{
	control = new RenderControl(this);

	mainSizer = new wxBoxSizer( wxVERTICAL );
	mainSizer->Add( control, 1, wxEXPAND );
	
	SetSizerAndFit( mainSizer );
	Layout();
}

//-----------------------------------//

Viewframe::~Viewframe()
{
	control->stopFrameLoop();
	control->Destroy();
}

//-----------------------------------//

Viewport* Viewframe::createViewport( NodePtr node )
{
	assert( node != nullptr );

	// Add a new viewport to the window.
	CameraPtr camera = node->getComponent<Camera>("Camera");
	assert( camera != nullptr );

	Window* window = control->getRenderWindow();
	viewport = window->addViewport( camera );

	// Subscribe to the camera transform events.
	TransformPtr transform = node->getTransform();
	transform->onTransform += fd::bind( &Viewframe::flagRedraw, this );
	
	return viewport;
}

//-----------------------------------//

void Viewframe::flagRedraw()
{
	if( control )
		control->flagRedraw();
}

//-----------------------------------//

} } // end namespaces
