/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "vapor/PCH.h"
#include "MeshBuilder.h"
#include "Render/Renderable.h"
#include "Math/Vector2.h"
#include "Math/Helpers.h"
#include "Utilities.h"

namespace vapor {

//-----------------------------------//

std::map<MeshPtr, std::vector<RenderablePtr> > MeshBuilder::meshRenderables;

MeshBuilder::MeshBuilder()
{ }

//-----------------------------------//

bool MeshBuilder::build(const MeshPtr& mesh)
{
	if( mesh->isBuilt() )
		return true;

	this->mesh = mesh;

	mesh->buildBounds();
	mesh->setupInitialVertices();	
	buildGeometry();

	mesh->built = true;

	return true;
}

//-----------------------------------//

void MeshBuilder::buildGeometry()
{
	// Vertex buffer.
	VertexBufferPtr vb = new VertexBuffer();

	vb->set( VertexAttribute::Position, mesh->position );
	vb->set( VertexAttribute::Normal, mesh->normals );
	vb->set( VertexAttribute::TexCoord0, mesh->texCoords );

	if( mesh->isAnimated() )
	{
		vb->set( VertexAttribute::BoneIndex, mesh->boneIndices );
	}

	// Construct the mesh groups.
	const std::vector<MeshGroup>& groups = mesh->groups;

	for( uint i = 0; i < groups.size(); i++ )
	{
		const MeshGroup& group = groups[i];

		// Gets a material for the group.
		MaterialPtr mat = buildMaterial(group);

		IndexBufferPtr ib = new IndexBuffer();
		ib->set(group.indices);

		RenderablePtr renderable = new Renderable();
		renderable->setPrimitiveType( PolygonType::Triangles );
		renderable->setVertexBuffer(vb);
		renderable->setIndexBuffer(ib);
		renderable->setMaterial(mat);
		meshRenderables[mesh].push_back(renderable);

		#pragma TODO("Use index buffers when building mesh geometry")
	}
}

//-----------------------------------//
	
MaterialPtr MeshBuilder::buildMaterial(const MeshGroup& group)
{
	const MeshMaterial& matMesh = group.material;

	MaterialPtr material = new Material(matMesh.name);

	if( !matMesh.texture.empty() )
	{
		material->setProgram("Tex_Toon");

		std::string path = PathUtils::normalize(matMesh.texture);
		material->setTexture( 0, path );
	}

	if( matMesh.alpha )
	{
		#pragma TODO("Use alpha testing when alpha values are fully transparent.")
		material->setAlphaTest(true);
		//mat->setBlending( BlendSource::SourceAlpha, BlendDestination::InverseSourceAlpha );
	}

	if( mesh->isAnimated() )
		material->setProgram("Tex_Toon_Skin");

	return material;
}

//-----------------------------------//

} // end namespace