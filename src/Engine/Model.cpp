/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "vapor/PCH.h"
#include "vapor/scene/Model.h"
#include "vapor/resources/Mesh.h"
#include "vapor/animation/Animation.h"
#include "vapor/animation/Skeleton.h"
#include "vapor/animation/Bone.h"

namespace vapor {

//-----------------------------------//

static const uint MAX_BONES = 32;

//-----------------------------------//

BEGIN_CLASS_PARENT(Model, Geometry)
	FIELD_PRIMITIVE(Model, float, animationSpeed)
	FIELD_CLASS_PTR(Model, Mesh, mesh)
END_CLASS()

//-----------------------------------//

Model::Model()
{
	init();
}

//-----------------------------------//

Model::Model( const MeshPtr& mesh )
	: mesh(mesh)
{
	init();
}

//-----------------------------------//

void Model::init()
{
	modelBuilt = false;
	needsRenderCallback = true;
	debugRenderable = nullptr;

	animationTime = 0;
	animationSpeed = 1;
	animationEnabled = true;
}

//-----------------------------------//

void Model::update( double delta )
{
	if( !mesh )
		return;

	if( !mesh->isLoaded() )
		return;

	if( !modelBuilt )
		build();

	if( mesh->isAnimated() )
	{
		animation = mesh->getAnimations()[0];

		if( animationEnabled )
			updateAnimation();

		advanceTime(delta);
	}

	Geometry::update(delta);
}

//-----------------------------------//

void Model::updateBounds()
{
	boundingVolume = mesh->getBoundingVolume();
}

//-----------------------------------//

void Model::advanceTime( double delta )
{
	animationTime += delta * (10 * animationSpeed);

	if( animationTime > animation->getTotalTime() )
	{
		if( animation->isLooped() )
			animationTime = 0;
		else
			animationEnabled = false;
	}
}

//-----------------------------------//

void Model::switchAnimation(const std::string& name)
{
	assert( mesh != nullptr );

	if( !mesh->isAnimated() )
		return;

	animation = mesh->findAnimation(name);
}

//-----------------------------------//

void Model::updateAnimation()
{
	if( !animation )
		return;

	updateBoneMatrices();
}

//-----------------------------------//

void Model::updateBoneMatrices()
{
	const KeyFramesMap& keyFrames = animation->getKeyFrames();

	foreach( const KeyFramesPair& p, keyFrames )
	{
		const BonePtr& bone = p.first;
		updateBoneMatrix( bone );
	}
}

//-----------------------------------//

void Model::updateBoneMatrix(const BonePtr& bone)
{
	Matrix4x3 frameMatrix =
		animation->getInterpolatedKeyFrameMatrix(bone, animationTime);
	
	frameMatrix = frameMatrix * bone->relativeMatrix;

	if( bone->parentIndex != -1 )
		bonesMatrix[bone->index] = frameMatrix * bonesMatrix[bone->parentIndex];
	else
		bonesMatrix[bone->index] = frameMatrix;
}

//-----------------------------------//

RenderablePtr Model::getDebugRenderable() const
{
	if( !mesh->isAnimated() )
		return nullptr;

	if( !debugRenderable )
		createDebugRenderable();

	VertexBufferPtr vb = debugRenderable->getVertexBuffer();

	std::vector<Vector3> pos;
	std::vector<Vector3> colors;

	foreach( const BonePtr& bone, mesh->getSkeleton()->getBones() )
	{
		Vector3 vertex;
		pos.push_back( bonesMatrix[bone->index]*vertex );
		colors.push_back( Color::Blue );

		if( bone->parentIndex != -1 )
		{
			Vector3 parentVertex;
			pos.push_back( bonesMatrix[bone->parentIndex]*parentVertex );
			colors.push_back( Color::Blue );
		}
	}

	vb->set( VertexAttribute::Position, pos );
	vb->set( VertexAttribute::Color, colors );

	return debugRenderable;
}

//-----------------------------------//

void Model::createDebugRenderable() const
{
	assert( !debugRenderable );

	MaterialPtr mat = new Material("Skeleton");
	mat->setProgram("Diffuse");
	mat->setDepthTest(false);

	VertexBufferPtr vb = new VertexBuffer();

	debugRenderable = new Renderable(PolygonType::LineStrip, vb, mat);
}

//-----------------------------------//

void Model::build()
{
	if( modelBuilt )
		return;

	std::vector<RenderablePtr> renderables;
	mesh->appendRenderables( renderables );

	foreach( const RenderablePtr& rend, renderables )
	{
		MaterialPtr material = rend->getMaterial();

		if( material && material->isBlendingEnabled() )
			addRenderable( rend, RenderStage::Transparency );
		else
			addRenderable( rend );
	}

	if( mesh->isAnimated() )
		bonesMatrix.resize(MAX_BONES);	

	modelBuilt = true;
}

//-----------------------------------//

void Model::onRender()
{
	const RenderablePtr& rend = getRenderables()[0];
	const MaterialPtr& mat = rend->getMaterial();
	const ProgramPtr& prog = mat->getProgram();

	std::vector<Matrix4x4> newMatrices;
	newMatrices.reserve( bonesMatrix.size() );

	foreach( const Matrix4x3& mat, bonesMatrix )
		newMatrices.push_back(Matrix4x4(mat));
	
	prog->setUniform("vp_BonesMatrix", newMatrices);
}

//-----------------------------------//

} // end namespace