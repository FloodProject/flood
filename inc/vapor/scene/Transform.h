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

class Transformable : public Node, public TransformInterface
{
public:

	virtual void translate( const math::Vector3& tr );
  
	virtual void rotate( float xang, float yang, float zang );
  
	// many more methods implemented here ...

protected:

	Transformable();
	~Transformable();
  
private:

	//shared_ptr<math::Vector3> translation;
	//shared_ptr<math::Matrix4> orientation;
	math::Vector3 translation;
	math::Matrix4 orientation;
};


//-----------------------------------//

class Transform : public Group, public TransformInterface
{
public:

};

//-----------------------------------//

} } // end namespaces