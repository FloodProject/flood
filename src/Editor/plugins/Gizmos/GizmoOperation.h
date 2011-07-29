/************************************************************************
*
* vapor3D Editor © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "Gizmo.h"
#include "GizmoPlugin.h"
#include "UndoOperation.h"

NAMESPACE_EDITOR_BEGIN

//-----------------------------------//

class GizmoOperation : public UndoOperation
{
public:

	GizmoOperation();
	
	void undo();
	void redo();
	void process( bool undo );

	EntityWeakPtr weakEntity;
	GizmoTool::Enum tool;
	GizmoAxis::Enum axis;
	GizmoPtr gizmo;

	Vector3 prevTranslation;
	Quaternion prevRotation;
	Vector3 prevScale;

	Vector3 translation;
	Quaternion rotation;
	Vector3 scale;
};

//-----------------------------------//

NAMESPACE_EDITOR_END