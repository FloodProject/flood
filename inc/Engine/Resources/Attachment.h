/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

#include "Core/Math/Vector.h"
#include "Core/Math/Quaternion.h"

FWD_DECL_INTRUSIVE(Bone)

NAMESPACE_ENGINE_BEGIN

//-----------------------------------//

/**
 * Attachments allow meshes to be combined onto other meshes.
 * For example, you can attach a sword to the hand of a ninja.
 */

class API_ENGINE Attachment : public ReferenceCounted
{
public:

	// Attachment bone.
	BonePtr bone;

	// Attachment node.
	EntityPtr node;

	// Position offset.
	Vector3 offsetPosition;

	// Orientation offset.
	Quaternion offsetOrientation;
};

TYPEDEF_RESOURCE_HANDLE_FROM_TYPE( Attachment ) ;

//-----------------------------------//

NAMESPACE_ENGINE_END
