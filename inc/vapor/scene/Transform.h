/************************************************************************
*
* vaporEngine (2008-2009)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#pragma once

#include "vapor/Platform.h"

#include "vapor/scene/Node.h"
#include "vapor/scene/Group.h"

#include "vapor/math/Matrix.h"
#include "vapor/math/Vector3.h"

namespace vapor {
	namespace scene {

//-----------------------------------//

/*
 * This is purely an interface, contains no data or implementation
 */
class TransformInterface
{
public:

	virtual void translate( math::Vector3 const& tr ) = 0;
  
	virtual void rotate( float xang, float yang, float zang ) = 0;
  
	// many more methods follow here ...
};

//-----------------------------------//

class TransformableImplementation : public TransformInterface
{
public:

	virtual void translate( math::Vector3 const& tr );
  
	virtual void rotate( float xang, float yang, float zang );
  
	// many more methods implemented here ...

protected:

	TransformableImplementation();
  
private:

	//shared_ptr<Vector3> translation;
	//shared_ptr<Matrix3x3> orientation;
};

//-----------------------------------//

class Transformable : public Node, public TransformableImplementation
{
};

//-----------------------------------//

class Transform : public Group, public TransformInterface
{
public:

	virtual void translate( math::Vector3 const& tr );
  
	virtual void rotate( float xang, float yang, float zang );

	// many more methods implemented here ...
  
private:
	math::Matrix4 matrix; 
};

//-----------------------------------//

} } // end namespaces