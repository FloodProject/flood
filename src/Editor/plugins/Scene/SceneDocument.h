/************************************************************************
*
* vapor3D Editor © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "Document.h"
#include "Viewframe.h"
#include "RenderWindow.h"
#include "RenderControl.h"

namespace vapor { namespace editor {

//-----------------------------------//

class SceneDocument : public Document 
{
public:

	SceneDocument();
	virtual ~SceneDocument();

	// Gets the document window.
	virtual wxWindow* getWindow();
	
	Viewframe* getViewframe() { return viewFrame;; }
	RenderControl* getRenderControl() { return viewFrame->getControl(); }
	RenderWindow* getRenderWindow() { return getRenderControl()->getRenderWindow(); }
	
	// Creates a context toolbar.
	virtual wxAuiToolBar* createContextToolbar() OVERRIDE;

	// Tool selection callbacks.
	virtual void onToolSelect(PluginTool* tool) OVERRIDE;

	// Document selection callbacks.
	virtual void onDocumentSelect();
	virtual void onDocumentUnselect();

	// View callbacks.
	void onUpdate( float delta );
	void onRender();

	void OnMouseRightUp(wxMouseEvent& event);
	void OnMouseRightDown(wxMouseEvent& event);
	void OnMouseEvent(wxMouseEvent& event);

	ScenePtr scene;
	ScenePtr editorScene;
	FirstPersonControllerPtr cameraController;
	
	Viewframe* viewFrame;
	wxAuiToolBar* toolbar;

protected:

	void setupRenderWindow();
	void createView();
	void createScene();
	EntityPtr createCamera();
};

//-----------------------------------//

} } // end namespaces