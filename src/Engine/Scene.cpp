/************************************************************************
*
* vaporEngine (2008-2009)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#include "vapor/PCH.h"

#include "vapor/scene/Scene.h"

#include <sstream>

using namespace vapor::math;

namespace vapor {
	namespace scene {

//-----------------------------------//

Scene::~Scene()
{

}

//-----------------------------------//

void Scene::update()
{
 //   std::stack< Matrix4 > transformStack;
 //   transformStack.push( Matrix4::identity() );
 //       
 //   updateTransformAndBV( this, transformStack );

	Group::update();
}

//-----------------------------------//

void Scene::updateTransformAndBV( NodePtr node, std::stack< Matrix4 >& transformStack )
{
    //// on the way down part
    //shared_ptr< Transformable > transformable(
    //  dynamic_cast< shared_ptr< Transformable > >( node) );
    //bool needsPop( false );
    //
    //// concatenate this matrix on the stack with the top most
    //// there will be at least one matrix in stack, top most is identity
    //if ( !transformable.isNull() )
    //{
    //    transformStack.push( transformable.getMatrix() * transformStack.top() );
    //    transformable.setAbsoluteLocalToWorld( transformStack.top() );
    //    needsPop = true;
    //}
    //else
    //{
    //    shared_ptr< Transform > transform(
    //      dynamic_cast< shared_ptr< Transform > >( node) );
    //
    //    // concatenate this matrix on the stack with the top most
    //    // there will be at least one matrix in stack, top most is identity
    //    if ( !transform.isNull() )
    //    {
    //        transformStack.push( transform.getMatrix() * transformStack.top() );
    //        needsPop = true;
    //    }
    //}
    //
    //for ( int i( 0 ); i < node->count(); ++i )
    //{
    //    updateTransformAndBV( node->get( i ) );
    //}
   
    //// on the way up part
    //if ( node->requiresBoundingVolumeUpdate() )
    //{
    //    node->updateBoundingVolume( transformStack.top() );
    //}
 
    //node->update();
    //    
    //// remove the matrix from the stack
    //if ( needsPop )
    //{
    //    transformStack.pop();
    //}
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