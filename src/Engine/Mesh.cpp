/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "vapor/PCH.h"
#include "vapor/resources/Mesh.h"
#include "vapor/animation/Skeleton.h"
#include "vapor/animation/Animation.h"
#include "vapor/render/Renderable.h"

namespace vapor {

//-----------------------------------//

BEGIN_CLASS_PARENT_ABSTRACT(Mesh, Resource)
END_CLASS()

//-----------------------------------//

Mesh::Mesh()
	: animated(false)
	, built(false)
	, bindPose(nullptr)
{ }

//-----------------------------------//

bool Mesh::isBuilt() const
{
	return built;
}

//-----------------------------------//

bool Mesh::isAnimated() const
{
	return animated;
}

//-----------------------------------//

AnimationPtr Mesh::findAnimation( const std::string& name )
{
	foreach( const AnimationPtr& animation, animations )
	{
		if( animation->getName() == name )
			return animation;
	}

	return nullptr;
}

//-----------------------------------//

void Mesh::appendRenderables( std::vector<RenderablePtr>& rends )
{
	if( !built )
		build();

	rends = renderables;
}

//-----------------------------------//

} // end namespace