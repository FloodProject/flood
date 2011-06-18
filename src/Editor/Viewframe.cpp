/************************************************************************
*
* vapor3D Editor © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Editor/API.h"
#include "Viewframe.h"
#include "RenderWindow.h"
#include "RenderControl.h"

namespace vapor { namespace editor {

//-----------------------------------//

Viewframe::Viewframe( wxWindow* parent, wxWindowID id,
					 const wxPoint& pos, const wxSize& size, long style ) 
	: wxPanel(parent, id, pos, size, style | wxBORDER_NONE | wxFULL_REPAINT_ON_RESIZE)
{
	mainSizer = new wxBoxSizer( wxVERTICAL );
	SetSizer( mainSizer );
}

//-----------------------------------//

Viewframe::~Viewframe()
{
	control->stopFrameLoop();
	control->Destroy();
}

//-----------------------------------//

RenderControl* Viewframe::createControl()
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
	mainSizer->Add( control, 1, wxEXPAND );

	return control;
}

//-----------------------------------//

void Viewframe::switchToDefaultCamera()
{
	const CameraPtr& camera = mainCamera;
	if( !camera ) return;

#if 0
	EntityPtr entityCamera = camera->getEntity();
	ControllerPtr controller = entityCamera->getComponentFromFamily<Controller>();
	if( controller ) controller->setEnabled(true);
#endif

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
	if( !control ) return;
	control->flagRedraw();
	//LogDebug("Force redraw of view");
}

//-----------------------------------//

} } // end namespaces
