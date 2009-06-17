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

using namespace vapor::math;

namespace vapor {
	namespace scene {

/**
 * Scene tree that holds all the nodes attached to the scene. This is the
 * main interface that the engine programmer will use to attach/detach all
 * different types of content to the scene. There are different types of 
 * nodes that can be used in the scene, like Mesh, Light, Camera, Particle,
 * Listener, Trigger, Billboard and maybe other that we want to later add.
 */

class Scene
{
public:

	Scene();
	virtual ~Scene();

	// Returns the root node of the scene tree
	Node* getRoot();

	// Node creation methods
	
	//Camera* createCamera();
	//Light* createLight();
	//Listener* createListener();
	//etc...

protected:

	Node* rootNode;

};

} } // end namespaces