/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Engine/API.h"
#include "Render/MeshManager.h"
#include "Render/Renderable.h"

NAMESPACE_ENGINE_BEGIN

//-----------------------------------//

bool MeshManager::build(Mesh* mesh)
{
	if( !mesh ) return false;

	if( !mesh->isBuilt() )
	{
		mesh->buildBounds();
		mesh->setupInitialVertices();
		mesh->built = true;
	}

	buildGeometry(mesh);

	return true;
}

//-----------------------------------//

void MeshManager::buildGeometry(Mesh* mesh)
{
	// Vertex buffer.
	VertexBufferPtr vb = Allocate(VertexBuffer, AllocatorGetHeap());

	vb->set( VertexAttribute::Position, mesh->position );
	vb->set( VertexAttribute::Normal, mesh->normals );
	vb->set( VertexAttribute::TexCoord0, mesh->texCoords );

	if( mesh->isAnimated() )
		vb->set( VertexAttribute::BoneIndex, mesh->boneIndices );

	// Construct the mesh groups.
	const std::vector<MeshGroup>& groups = mesh->groups;

	for( size_t i = 0; i < groups.size(); i++ )
	{
		const MeshGroup& group = groups[i];

		// Gets a material for the group.
		MaterialHandle material = buildMaterial(mesh, group);

		IndexBufferPtr ib = Allocate(IndexBuffer, AllocatorGetHeap());
		SetIndexBufferData(ib, group.indices);

		RenderablePtr renderable = Allocate(Renderable, AllocatorGetHeap());
		renderable->setPrimitiveType( PolygonType::Triangles );
		renderable->setVertexBuffer(vb);
		renderable->setIndexBuffer(ib);
		renderable->setMaterial(material);

		meshRenderables[mesh].push_back(renderable);

		#pragma TODO("Use index buffers when building mesh geometry")
	}
}

//-----------------------------------//
	
MaterialHandle MeshManager::buildMaterial(Mesh* mesh, const MeshGroup& group)
{
	const MeshMaterial& matMesh = group.material;

	MaterialHandle handle = MaterialCreate(AllocatorGetHeap(), matMesh.name);
	Material* material = handle.Resolve();

	if( !matMesh.texture.empty() )
	{
		material->setProgram("VertexLit");

		String path = PathNormalize(matMesh.texture);
		material->setTexture( 0, path );
	}

	if( matMesh.alpha )
	{
		#pragma TODO("Use alpha testing when alpha values are fully transparent.")
		material->setAlphaTest(true);
		//mat->setBlending( BlendSource::SourceAlpha, BlendDestination::InverseSourceAlpha );
	}

	if( mesh->isAnimated() )
		material->setProgram("VertexLitSkinned");

	return handle;
}

//-----------------------------------//

NAMESPACE_ENGINE_END