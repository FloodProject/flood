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

NAMESPACE_EDITOR_BEGIN

//-----------------------------------//

REFLECT_CHILD_CLASS(EntityOperation, UndoOperation)
REFLECT_CLASS_END()

//-----------------------------------//

EntityOperation::EntityOperation()
	: entity(nullptr)
	, component(nullptr)
	, weakScene(nullptr)
{
}

//-----------------------------------//

void EntityOperation::redo()
{
	ScenePtr scene = weakScene;
	
	if(!scene) return;
	
	switch(type)
	{
	case EntityAdded:
		scene->add(entity);
		break;
	case EntityRemoved:
		scene->remove(entity);
		break;
	case ComponentAdded:
		entity->addComponent(component);
		break;
	case ComponentRemoved:
		entity->removeComponent(component);
		break;
	}
}

//-----------------------------------//

void EntityOperation::undo()
{
	ScenePtr scene = weakScene;
	
	if(!scene) return;
	
	switch(type)
	{
	case EntityAdded:
		scene->remove(entity);
		break;
	case EntityRemoved:
		scene->add(entity);
		break;
	case ComponentAdded:
		entity->removeComponent(component);
		break;
	case ComponentRemoved:
		entity->addComponent(component);
		break;
	}
}

//-----------------------------------//

EntityOperation* ScenePage::createEntityOperation(const String& desc)
{
	EntityOperation* entityOperation = AllocateThis(EntityOperation);
	entityOperation->weakScene = weakScene;
	entityOperation->description = desc;

	return entityOperation;
}

//-----------------------------------//

NAMESPACE_EDITOR_END