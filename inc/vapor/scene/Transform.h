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

#include "vapor/math/Matrix.h"
#include "vapor/math/Vector3.h"

namespace vapor {
	namespace scene {

//-----------------------------------//

class VAPOR_API Transformable : public Node/*, public TransformInterface*/
{
public:

	virtual void translate( const math::Vector3& tr );
	virtual void rotate( float xang, float yang, float zang );
  
	// many more methods implemented here ...

	virtual void yaw( float ang );
	virtual void pitch( float ang );
	virtual void roll( float ang );

  // todo: identity()

	void setAbsoluteLocalToWorld( const math::Matrix4& matrix );
	const math::Matrix4& getAbsoluteLocalToWorld() const;


	const math::Matrix4& getLocalTransform() const;

protected:

	Transformable();
	~Transformable();

	//math::Vector3 translation;
	math::Matrix4 transform;
	math::Matrix4 absoluteLocalToWorld;
};

//-----------------------------------//

typedef tr1::shared_ptr< Transformable > TransformablePtr;

//-----------------------------------//

} } // end namespaces