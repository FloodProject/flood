/************************************************************************
*
* vapor3D Editor © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Editor/API.h"
#include "GizmoOperation.h"
#include "GizmoPlugin.h"

NAMESPACE_EDITOR_BEGIN

//-----------------------------------//

REFLECT_CHILD_CLASS(GizmoOperation, UndoOperation)
REFLECT_CLASS_END()

//-----------------------------------//

GizmoOperation::GizmoOperation()
	: tool(GizmoTool::Camera)
	, scale(1.0f, 1.0f, 1.0f)
{ }

//-----------------------------------//

void GizmoOperation::undo()
{ 
	process( true );
}

//-----------------------------------//

void GizmoOperation::redo()
{
	process( false );
}

//-----------------------------------//

void GizmoOperation::process( bool undo )
{
	EntityPtr node( weakEntity );

	// This can happen if the node gets deleted between
	// the operation registration and the undo/redo action.
	if( !node )
		return;

	TransformPtr transform = node->getTransform();
	
	transform->setPosition( undo ? prevTranslation : translation );
	transform->setRotation( undo ? prevRotation : rotation );
	transform->setScale( undo ? prevScale : scale );
}

//-----------------------------------//

NAMESPACE_EDITOR_END