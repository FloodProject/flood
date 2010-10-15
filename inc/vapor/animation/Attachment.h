/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#pragma once

#include "vapor/math/Vector3.h"
#include "vapor/math/Quaternion.h"

FWD_DECL_INTRUSIVE(Bone)

namespace vapor {

//-----------------------------------//

/**
 * Attachments allow meshes to be combined onto other meshes.
 * For example, you can attach a sword to the hand of a ninja.
 */

class VAPOR_API Attachment : public ReferenceCounted
{
public:

	// Attachment bone.
	BonePtr bone;

	// Attachment node.
	NodePtr node;

	// Position offset.
	Vector3 offsetPosition;

	// Orientation offset.
	Quaternion offsetOrientation;
};

TYPEDEF_INTRUSIVE_POINTER_FROM_TYPE(Attachment)

//-----------------------------------//

} // end namespace