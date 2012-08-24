/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Engine/API.h"
#include "Graphics/RenderBatch.h"
#include "Resources/Mesh.h"
#include "Resources/Animation.h"
#include "Resources/Skeleton.h"

NAMESPACE_ENGINE_BEGIN

//-----------------------------------//
	
static MaterialHandle MeshBuildMaterial(Mesh* mesh, const MeshGroup& group)
{
	const MeshMaterial& matMesh = group.material;

	MaterialHandle handle = MaterialCreate(AllocatorGetHeap(), matMesh.name);
	Material* material = handle.Resolve();

	if( !matMesh.texture.empty() )
	{
		material->setShader("VertexLit");

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
		material->setShader("VertexLitSkinned");

	return handle;
}

//-----------------------------------//

static void MeshBuildGeometry(Mesh* mesh, RenderablesVector& rends)
{
	GeometryBuffer* gb = mesh->getGeometryBuffer().get();
	
	// Construct the.renderables for each mesh group.
	const std::vector<MeshGroup>& groups = mesh->groups;

	for( size_t i = 0; i < groups.size(); i++ )
	{
		const MeshGroup& group = groups[i];

		uint32 numIndices = group.indices.size();
		if( numIndices == 0 ) continue;

		// Gets a material for the group.
		MaterialHandle material = MeshBuildMaterial(mesh, group);

		gb->addIndex((uint8*)&group.indices.front(), numIndices*sizeof(uint16));

		RenderBatch* renderable = AllocateHeap(RenderBatch);
		renderable->setPrimitiveType( PrimitiveType::Triangles );
		renderable->setGeometryBuffer(gb);
		renderable->setMaterial(material);

		Material* mat = material.Resolve();
		
		if( mat->isBlendingEnabled() )
		renderable->setRenderLayer(RenderLayer::Transparency);

		rends.push_back(renderable);
	}
}

//-----------------------------------//

bool MeshBuild(Mesh* mesh, RenderablesVector& rends)
{
	if( !mesh ) return false;

	if( !mesh->isBuilt() )
	{
		mesh->buildBounds();
		mesh->setupInitialVertices();
		mesh->built = true;
	}

	MeshBuildGeometry(mesh, rends);

	return true;
}

//-----------------------------------//

NAMESPACE_ENGINE_END