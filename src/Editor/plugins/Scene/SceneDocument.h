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

class RenderControl;
class ReplicaContext;

NAMESPACE_EDITOR_BEGIN

//-----------------------------------//

class SceneDocument : public Document 
{
public:

	SceneDocument();
	virtual ~SceneDocument();

	// Document management callbacks.
	bool open() OVERRIDE;
	bool save() OVERRIDE;
	bool reset() OVERRIDE;

	// Document callbacks.
	void onDocumentSelect() OVERRIDE;
	void onDocumentUnselect() OVERRIDE;
	void onToolSelect(PluginTool* tool) OVERRIDE;

	// Gets the document window.
	wxWindow* getWindow() OVERRIDE;

	// Resets the scene.
	void resetScene();

	// Sets the current scene.
	void setScene( Scene* scene );

	// Gets the view controls.
	Viewframe* getViewframe();
	RenderControl* getRenderControl();
	RenderWindow* getRenderWindow();
	
	// Creates a context toolbar.
	wxAuiToolBar* createContextToolbar() OVERRIDE;

	ReplicaContext* replicaContext;

	ScenePtr scene;
	ScenePtr editorScene;
	FirstPersonControllerPtr cameraController;
	
	Viewframe* viewframe;
	wxAuiToolBar* toolbar;

protected:

	// View callbacks.
	void onUpdate( float delta );
	void onRender();

	void createView();
	void setupRenderWindow();

	void OnMouseRightUp(wxMouseEvent& event);
	void OnMouseRightDown(wxMouseEvent& event);
	void OnMouseEvent(wxMouseEvent& event);
	
	void createEditorScene();
	EntityPtr createCamera();
};

//-----------------------------------//

NAMESPACE_EDITOR_END