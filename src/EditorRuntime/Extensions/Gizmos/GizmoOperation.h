/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

#include "Gizmo.h"
#include "GizmoPlugin.h"
#include "UndoOperation.h"

NAMESPACE_EDITOR_BEGIN

//-----------------------------------//

REFLECT_DECLARE_CLASS(GizmoOperation)

class GizmoOperation : public UndoOperation
{
	REFLECT_DECLARE_OBJECT(GizmoOperation)

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