/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Engine/API.h"
#include "Scene/Projector.h"
#include "Scene/Transform.h"
#include "Scene/Entity.h"
#include "Render/DebugGeometry.h"
#include "Render/Device.h"
#include "Render/Program.h"
#include "Render/ProgramManager.h"

NAMESPACE_ENGINE_BEGIN

//-----------------------------------//

REFLECT_CHILD_CLASS(Projector, Geometry)
	FIELD_CLASS(4, Frustum, frustum)
	FIELD_CLASS_PTR(5, Material, MaterialHandle, material, Handle)
REFLECT_CLASS_END()

//-----------------------------------//

Projector::Projector()
{

}

//-----------------------------------//

Projector::~Projector()
{
	if( !geometry ) return;

	const RenderableVector& renderables = geometry->getRenderables();
	
	for( size_t i = 0; i < renderables.size(); i++ )
	{
		const RenderablePtr& renderable = renderables[i];
		renderable->onPreRender.clear();

		const UniformBufferPtr& ub = renderable->getUniformBuffer();
		ub->removeUniform("vp_TextureProjection");
		ub->removeUniform("vp_TextureView");
	}
}

//-----------------------------------//

void Projector::update(float)
{
	const TransformPtr& transform = getEntity()->getTransform();

	frustum.updateProjection();
	frustum.updateCorners(transform->getAbsoluteTransform());

	updateDebugRenderable();
}

//-----------------------------------//
 
void Projector::appendRenderables( RenderQueue& queue, const Transform* transform )
{
	if( !geometry ) return;

	const TransformPtr& geotransform = geometry->getEntity()->getTransform();
	const Matrix4x3& absoluteTransform = geotransform->getAbsoluteTransform();
	
	const RenderableVector& renderables = geometry->getRenderables();
	for( size_t i = 0; i < renderables.size(); i++ )
	{
		const RenderablePtr& renderable = renderables[i];
		if( !renderable ) continue;

		renderable->onPreRender.Bind(this, &Projector::onPreRender);

		RenderState state( renderable );
		state.material = material.Resolve();
		state.modelMatrix = absoluteTransform;
		state.priority = renderable->getRenderPriority()+1;

		queue.push_back(state);
	}
}

//-----------------------------------//

void Projector::onPreRender( RenderView*, const RenderState& state )
{
	const TransformPtr& transform = getEntity()->getTransform();
	const Matrix4x4& absoluteTransform = transform->getAbsoluteTransform();

	const UniformBufferPtr& ub = state.renderable->getUniformBuffer();
	ub->setUniform("vp_TextureProjection", frustum.matProjection);
	ub->setUniform("vp_TextureView", absoluteTransform.inverse());
}

//-----------------------------------//

void Projector::updateDebugRenderable() const
{
	if( !debugRenderable ) return;
	DebugUpdateFrustum( debugRenderable, frustum );
}

//-----------------------------------//

RenderablePtr Projector::createDebugRenderable() const
{
	assert( !debugRenderable );

	debugInheritsTransform = true;
	return DebugBuildFrustum( frustum );
}

//-----------------------------------//

NAMESPACE_ENGINE_END