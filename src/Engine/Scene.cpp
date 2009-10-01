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

#include <sstream>

using namespace vapor::math;

namespace vapor {
	namespace scene {

//-----------------------------------//

Scene::~Scene()
{

}

//-----------------------------------//

void Scene::update( double delta )
{
	std::stack< Matrix4 > transformStack;
	transformStack.push( Matrix4::Identity );
	    
	updateTransformAndBV( shared_from_this(), transformStack );

	Group::update( delta );
}

//-----------------------------------//

void Scene::updateTransformAndBV( NodePtr node, std::stack< Matrix4 >& transformStack )
{
	// TODO: benckmark and profile this, smells slow

    // on the way down part
	TransformablePtr transformable( 
		tr1::dynamic_pointer_cast< Transformable >( node ) );

	// this is used to know if we need to pop a matrix from the
	// stack on the way up part, because some nodes will not have
	// pushed matrices on the stack on the way down part.
	bool needsPop = false;
  
	// concatenate this matrix on the stack with the top most
	// there will be at least one matrix in stack, top most is identity
	if ( transformable )
	{
		transformStack.push( transformable->getLocalTransform() * transformStack.top() );
		transformable->setAbsoluteTransform( transformStack.top() );
		needsPop = true;
	}

	GroupPtr group( tr1::dynamic_pointer_cast< Group >( node ) );

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
	std::ostringstream os;
	os << "{ " << Group::save(ind);
	return os.str();
}

//-----------------------------------//

} } // end namespaces