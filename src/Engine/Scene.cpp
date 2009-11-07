/************************************************************************
*
* vaporEngine (2008-2009)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#include "vapor/PCH.h"

#include "vapor/scene/Scene.h"
#include "vapor/scene/Transform.h"

using namespace vapor::math;

namespace vapor {
	namespace scene {

//-----------------------------------//

Scene::Scene() : Group( "Scene" )
{

}

//-----------------------------------//

Scene::~Scene()
{

}

//-----------------------------------//

void Scene::update( float delta )
{
	std::stack< Matrix4x3 > transformStack;
	transformStack.push( Matrix4x3::Identity );
	    
	updateTransformAndBV( shared_from_this(), transformStack );

	Group::update( delta );
}

//-----------------------------------//

void Scene::updateTransformAndBV( NodePtr node, std::stack< Matrix4x3 >& transformStack )
{
	// TODO: benckmark and profile this, smells slow

    // on the way down part
	TransformPtr transform = node->getTransform();

	// this is used to know if we need to pop a matrix from the
	// stack on the way up part, because some nodes will not have
	// pushed matrices on the stack on the way down part.
	bool needsPop = false;
  
	// concatenate this matrix on the stack with the top most
	// there will be at least one matrix in stack, top most is identity
	if ( transform )
	{
		transformStack.push( transform->getLocalTransform() * transformStack.top() );
		transform->setAbsoluteTransform( transformStack.top() );
		needsPop = true;
	}

	GroupPtr group( std::dynamic_pointer_cast< Group >( node ) );

	if( group )
	{
		foreach( NodePtr gnode, group->getChildren() )
		{
			updateTransformAndBV( gnode, transformStack /*node->get( i )*/ );
		}
	}

	// on the way up part
	//if ( node->requiresBoundingVolumeUpdate() )
	//{
	//    node->updateBoundingVolume( transformStack.top() );
	//}

	//node->update();
	    
	// remove the matrix from the stack
	if ( needsPop )
	{
		transformStack.pop();
	}
}

//-----------------------------------//

const std::string Scene::save(int ind)
{
	//std::ostringstream os;
	//os << "{ " << Group::save(ind);
	//return os.str();
	return "";
}

//-----------------------------------//

Node* Scene::getEntity( const std::string& name ) const
{
	foreach( NodePtr gnode, getChildren() )
	{
		if( gnode->getName() == name )
			return gnode.get();
	}

	return nullptr;
}

//-----------------------------------//

} } // end namespaces