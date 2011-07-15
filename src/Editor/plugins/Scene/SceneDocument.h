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

NAMESPACE_EDITOR_BEGIN

//-----------------------------------//

class SceneDocument : public Document 
{
public:

	SceneDocument();
	virtual ~SceneDocument();

	// Resets the scene.
	void resetScene();

	// Gets the document window.
	virtual wxWindow* getWindow();
	
	// Gets the view frame.
	Viewframe* getViewframe() { return viewFrame; }

	// Gets the render control.
	RenderControl* getRenderControl() { return viewFrame->getControl(); }
	
	// Gets the render window.
	RenderWindow* getRenderWindow() { return getRenderControl()->getRenderWindow(); }
	
	// Creates a context toolbar.
	virtual wxAuiToolBar* createContextToolbar() OVERRIDE;

	// Tool selection callbacks.
	virtual void onToolSelect(PluginTool* tool) OVERRIDE;

	// Document selection callbacks.
	virtual void onDocumentSelect() OVERRIDE;
	virtual void onDocumentUnselect() OVERRIDE;

	ScenePtr scene;
	ScenePtr editorScene;
	FirstPersonControllerPtr cameraController;
	
	Viewframe* viewFrame;
	wxAuiToolBar* toolbar;

protected:

	// View callbacks.
	void onUpdate( float delta );
	void onRender();

	void OnMouseRightUp(wxMouseEvent& event);
	void OnMouseRightDown(wxMouseEvent& event);
	void OnMouseEvent(wxMouseEvent& event);

	void setupRenderWindow();
	void createView();
	void createScene();
	EntityPtr createCamera();
};

//-----------------------------------//

NAMESPACE_EDITOR_END