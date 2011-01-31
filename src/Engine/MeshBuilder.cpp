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

std::map<MeshPtr, std::vector<RenderablePtr>> MeshBuilder::meshRenderables;

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
	const std::vector<MeshGroup>& groups = mesh->groups;

	for( uint i = 0; i < groups.size(); i++ )
	{
		const MeshGroup& group = groups[i];

		// Gets a material for the group.
		MaterialPtr mat = buildMaterial(group);

		// Vertex buffer.
		VertexBufferPtr vb = new VertexBuffer();

		vb->set( VertexAttribute::Position, group.position );
		vb->set( VertexAttribute::Normal, group.normals );
		vb->set( VertexAttribute::TexCoord0, group.texCoords );

		if( mesh->isAnimated() )
			vb->set( VertexAttribute::BoneIndex, group.bones );

		// Renderable.
		RenderablePtr rend = new Renderable();
		rend->setPrimitiveType( PolygonType::Triangles );
		rend->setVertexBuffer( vb );
		rend->setMaterial( mat );
		
		meshRenderables[mesh].push_back( rend );

		#pragma TODO("Use index buffers when building mesh geometry")
		
		//std::vector< ushort > vb_i;
		//foreach( const ms3d_triangle_t& t, triangles )
		//{	
		//	vb_i.push_back( t.vertexIndices[0] );
		//	vb_i.push_back( t.vertexIndices[1] );
		//	vb_i.push_back( t.vertexIndices[2] );
		//}

		//IndexBufferPtr ib( new IndexBuffer() );
		//ib->set( vb_i );
		//rend->setIndexBuffer( ib );
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