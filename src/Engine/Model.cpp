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
	currentTime = 0;
	animate = true;
	renderable = nullptr;
	needsRenderCallback = true;
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
		currentTime += delta * 10;

		if( currentTime > animation->getTotalTime() )
		{
			if( animation->isLooped() )
				currentTime = 0;
			else
				animate = false;
		}

		if( animate )
			updateAnimation();
	}

	Geometry::update(delta);
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
		Matrix4x3 frameMatrix = animation->getInterpolatedKeyFrameMatrix(bone, currentTime);
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

	if( !renderable )
		createDebugRenderable();

	VertexBufferPtr vb = renderable->getVertexBuffer();

	std::vector<Vector3> pos;
	std::vector<Vector3> colors;

	foreach( const BonePtr& bone, mesh->getSkeleton()->getBones() )
	{
		Vector3 vertex;
		pos.push_back( vertex * bonesMatrix[bone->index] );
		colors.push_back( Color::Blue );

		if( bone->parentIndex != -1 )
		{
			Vector3 parentVertex;
			pos.push_back( parentVertex * bonesMatrix[bone->parentIndex] );
			colors.push_back( Color::Blue );
		}
	}

	vb->set( VertexAttribute::Position, pos );
	vb->set( VertexAttribute::Color, colors );

	return renderable;
}

//-----------------------------------//

void Model::createDebugRenderable() const
{
	assert( !renderable );

	MaterialPtr mat = new Material("Skeleton");
	mat->setProgram("Diffuse");
	mat->setDepthTest(false);

	VertexBufferPtr vb = new VertexBuffer();

	renderable = new Renderable(PolygonType::LineStrip, vb, mat);
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