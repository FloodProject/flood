/************************************************************************
*
* vaporEngine (2008-2009)
*
*	<http://www.portugal-a-programar.org>
*
************************************************************************/

#pragma once

#include "vapor/Platform.h"
#include "vapor/scene/Group.h"

#include "vapor/math/Matrix4x3.h"

namespace vapor { namespace scene {

//-----------------------------------//

/**
 * Scene tree that holds all the nodes attached to the scene. This is the
 * main interface that the engine programmer will use to attach/detach all
 * different types of content to the scene. There are different types of 
 * nodes that can be used in the scene, like Mesh, Light, Camera, Particle,
 * Listener, Trigger, Billboard and maybe other that we want to later add.
 */

class VAPOR_API Scene : public Group
{
public:

	Scene();
	virtual ~Scene();

	// Node creation methods

	//MeshPtr createMesh();
	//Camera* createCamera();
	//Light* createLight();
	//Listener* createListener();
	//etc...

	/// Updates all the entities recursively.
	virtual void update( float delta );

	/// Serializes the scene to a stream.
	virtual const std::string save( int indent = 0 );

	/// Gets a raw pointer to the named entity.
	Node* getEntityPtr( const std::string& name ) const;

	/// Gets a shared pointer to the named entity.
	NodePtr getEntity( const std::string& name ) const;

private:

	// Updates all the transforms and bounding volumes of the scene nodes.
	void updateTransformAndBV( NodePtr node, 
		std::stack< math::Matrix4x3 >& transformStack );
};

//-----------------------------------//

TYPEDEF_SHARED_POINTER_FROM_CLASS( Scene );
TYPEDEF_SHARED_WEAK_POINTER_FROM_CLASS( Scene );

//-----------------------------------//

} } // end namespaces