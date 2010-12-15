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
	int n = 0;
	int attribList[16];
	attribList[n++] = WX_GL_RGBA;
	attribList[n++] = WX_GL_DOUBLEBUFFER;
	attribList[n++] = WX_GL_DEPTH_SIZE;
	attribList[n++] = 16;
	attribList[n++] = WX_GL_SAMPLE_BUFFERS;
	attribList[n++] = 1;
	attribList[n++] = WX_GL_SAMPLES;
	attribList[n++] = 4;
	attribList[n] = 0; // terminate the list

	control = new RenderControl(this, wxID_ANY, attribList);

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

void Viewframe::switchToDefaultCamera()
{
	CameraPtr camera = mainCamera.lock();

	if( !camera )
		return;

	EntityPtr nodeCamera = camera->getEntity();
	ControllerPtr controller = nodeCamera->getTypedComponent<Controller>();

	if( controller )
		controller->setEnabled(true);

	view->setCamera(camera);
}

//-----------------------------------//

RenderView* Viewframe::createView( /*const EntityPtr& node*/ )
{
	// Add a new view to the window.
	//CameraPtr camera = node->getComponent<Camera>();
	//assert( camera != nullptr );

	Window* window = control->getRenderWindow();
	view = window->createView();
	//view->setCamera(camera);

	// Subscribe to the camera transform events.
	//TransformPtr transform = node->getTransform();
	//transform->onTransform.Connect( &Viewframe::flagRedraw, this );
	
	return view;
}

//-----------------------------------//

void Viewframe::flagRedraw()
{
	if( control )
		control->flagRedraw();
}

//-----------------------------------//

} } // end namespaces
