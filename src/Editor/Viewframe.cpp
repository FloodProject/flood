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
	int attribList[] =
	{
		WX_GL_RGBA,
		WX_GL_DOUBLEBUFFER,
		WX_GL_DEPTH_SIZE, 16,
		//WX_GL_SAMPLE_BUFFERS, 1,
		//WX_GL_SAMPLES, 4,
		0
	};

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
	const CameraPtr& camera = mainCamera.lock();

	if( !camera )
		return;

	EntityPtr nodeCamera = camera->getEntity()->getShared();
	ControllerPtr controller = nodeCamera->getComponentFromFamily<Controller>();

	if( controller )
		controller->setEnabled(true);

	view->setCamera(camera);
}

//-----------------------------------//

RenderView* Viewframe::createView()
{
	Window* window = control->getRenderWindow();
	view = window->createView();

	view->onCameraChanged.Connect(this, &Viewframe::onCameraChanged);
	
	return view;
}

//-----------------------------------//

void Viewframe::onCameraChanged(const CameraPtr& camera)
{
	flagRedraw();

	// Subscribe to the camera transform events.
	TransformPtr transform = camera->getEntity()->getTransform();
	transform->onTransform.Connect( this, &Viewframe::flagRedraw );
}

//-----------------------------------//

void Viewframe::flagRedraw()
{
	if( !control )
		return;

	control->flagRedraw();
	//Log::debug("Force redraw of view");
}

//-----------------------------------//

} } // end namespaces
