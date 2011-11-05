/************************************************************************
*
* vapor3D Editor © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "Document.h"
#include "SceneWindow.h"
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

	// Creates the document.
	DocumentWindow* createDocumentWindow() OVERRIDE;

	// Document lifetime callbacks.
	void onDocumentDestroy() OVERRIDE;

	// Document management callbacks.
	bool onDocumentOpen() OVERRIDE;
	bool onDocumentSave() OVERRIDE;
	bool onDocumentReset() OVERRIDE;

	// Document callbacks.
	void onDocumentSelect() OVERRIDE;
	void onDocumentUnselect() OVERRIDE;
	void onToolSelect(PluginTool* tool) OVERRIDE;

	// Scene management.
	void resetScene();
	void setScene( Scene* scene );

	const char* getFileDialogDescription() OVERRIDE;

	GETTER(Viewframe, SceneWindow*, sceneWindow)
	
	RenderControl* getRenderControl();
	RenderWindow* getRenderWindow();
	
	ReplicaContext* replicaContext;

	ScenePtr scene;
	ScenePtr editorScene;
	FirstPersonControllerPtr cameraController;
	
	SceneWindow* sceneWindow;
	wxAuiToolBar* toolbar;

protected:

	// View callbacks.
	void onUpdate( float delta );
	void onRender();

	void setupRenderWindow();

	void OnMouseRightUp(wxMouseEvent& event);
	void OnMouseRightDown(wxMouseEvent& event);
	void OnMouseEvent(wxMouseEvent& event);
	
	void createEditorScene();
	EntityPtr createCamera();
};

//-----------------------------------//

NAMESPACE_EDITOR_END