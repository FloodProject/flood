/************************************************************************
*
* vapor3D Editor © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Editor/API.h"
#include "ScenePane.h"
#include "UndoManager.h"
#include "Editor.h"

namespace vapor { namespace editor {

//-----------------------------------//

void EntityOperation::redo()
{
	ScenePtr scene = weakScene.lock();
	
	if(!scene) return;
	
	if(added)
		scene->add(entity);
	else
		scene->remove(entity);
}

//-----------------------------------//

void EntityOperation::undo()
{
	ScenePtr scene = weakScene.lock();
	
	if(!scene) return;
	
	if(added)
		scene->remove(entity);
	else
		scene->add(entity);
}

//-----------------------------------//

EntityOperation* ScenePage::createEntityOperation(const EntityPtr& entity, const std::string& desc)
{
	if( !entity ) return nullptr;

	EntityOperation* nodeOperation = new EntityOperation();
	nodeOperation->entity = entity;
	nodeOperation->weakScene = weakScene;
	nodeOperation->description = desc;

	return nodeOperation;
}

//-----------------------------------//

} } // end namespaces