/************************************************************************
*
* vapor3D Editor © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Editor/API.h"

#ifdef ENABLE_PLUGIN_TERRAIN

#include "TerrainOperation.h"

namespace vapor { namespace editor {

//-----------------------------------//

TerrainOperation::TerrainOperation( TerrainTool::Enum tool,
								    const RayTriangleQueryResult& res )
	: brushSize(0)
	, brushStrength(0)
	, tool(tool)
	, rayQuery(res)
{
	assert( res.renderable != nullptr );

	if( tool == TerrainTool::Raise )
		loadSaveHeights( beforeHeights, true );
	else if( tool == TerrainTool::Paint )
		loadSaveImage( beforeImage, true );
}

//-----------------------------------//

void TerrainOperation::ready()
{
	if( tool == TerrainTool::Raise )
	{
		loadSaveHeights( afterHeights, true );
		updateNormals();
	}
	else if( tool == TerrainTool::Paint )
	{
		loadSaveImage( afterImage, true );
	}
}

//-----------------------------------//

void TerrainOperation::undo()
{
	if( tool == TerrainTool::Raise )
	{
		loadSaveHeights( beforeHeights, false );
		updateNormals();
	}
	else if( tool == TerrainTool::Paint )
	{
		loadSaveImage( beforeImage, false );
	}
}

//-----------------------------------//

void TerrainOperation::redo()
{
	// If the operation can't be undone with an algorithm, 
	// then our best option is to save the state of the terrain.

	if( tool == TerrainTool::Raise )
	{
		loadSaveHeights( afterHeights, false );
		updateNormals();
	}
	else if( tool == TerrainTool::Paint )
	{
		loadSaveImage( afterImage, false );
	}
}

//-----------------------------------//

void TerrainOperation::updateNormals()
{
	//RenderablePtr renderable = rayQuery.renderable;

	#pragma TODO("Generate terrain normals in the background")
	//CellPtr cell = boost::static_pointer_cast<Cell>( renderable );

	const Vector3& pick = rayQuery.intersectionWorld;

	BoundingSphere bs( pick, brushSize );
	
	std::vector<CellPtr> cells;
	getCellsInRange(bs, cells);

	for( size_t i = 0; i < cells.size(); i++ )
	{
		const CellPtr& cell = cells[i];
		cell->rebuildNormals();
	}
}

//-----------------------------------//

void TerrainOperation::loadSaveHeights( std::vector<float>& heights, bool save )
{	
	RenderablePtr rend = rayQuery.renderable;
	VertexBufferPtr vb = rend->getVertexBuffer();
	
	if( save )
	{
		// We only need the height of the vertex. This way we can save some memory.
		#pragma TODO("There is no need to save the unmodified heights of the terrain")
		const std::vector<Vector3>& vertices = vb->getVertices();

		for( size_t i = 0; i < vertices.size(); i++ )
		{
			const Vector3& v = vertices[i];
			heights.push_back( v.y );
		}
	}
	else
	{
		std::vector<Vector3>& vertices = vb->getVertices();

		for( size_t i = 0; i < vertices.size(); i++ )
			vertices[i].y = heights[i];

		vb->forceRebuild();
	}
}

//-----------------------------------//

void TerrainOperation::loadSaveImage( std::vector<byte>& state, bool save )
{	
	RenderablePtr rend = rayQuery.renderable;
	MaterialPtr material = rend->getMaterial().Resolve();
	Image* texture = material->getTexture(0).Resolve();

	if( save )
	{
		//state = image->getBuffer();
	}
	else
	{
		//image->setBuffer(state);
		//texture->setImage(image);
	}
}

//-----------------------------------//

void TerrainOperation::applyTool()
{
	if( tool == TerrainTool::Raise )
		applyRaiseTool();
	else if( tool == TerrainTool::Paint )
		applyPaintTool();
}

//-----------------------------------//

void TerrainOperation::getCellsInRange(const BoundingSphere& bs, std::vector<CellPtr>& cells)
{
	if( !terrain ) return;

	const std::vector<EntityPtr>& entities = terrain->getEntities();

	for( size_t i = 0; i < entities.size(); i++ )
	{
		const EntityPtr& entity = entities[i];
		const TransformPtr& transform = entity->getTransform();
		const BoundingBox& box = transform->getWorldBoundingVolume();
		BoundingSphere cellSphere(box);
		
		if( !bs.intersects(cellSphere) )
			continue;

		const CellPtr& cell = entity->getComponent<Cell>();
		cells.push_back(cell);
		
		//GeometryPtr geometry = node->getComponent<Geometry>();
		//assert( geometry != nullptr );

		//const RenderableVector& renderables = geometry->getRenderables();
		//assert( !renderables.empty() );

		//const CellPtr& cell = RefCast<Cell>(renderables[0]);
		//cells.push_back(cell);
	}
}

//-----------------------------------//

void TerrainOperation::applyRaiseTool()
{
	RenderablePtr rend = rayQuery.renderable;
	const Vector3& pick = rayQuery.intersectionWorld;

	BoundingSphere bs( pick, brushSize );
	
	std::vector<CellPtr> cells;
	getCellsInRange(bs, cells);

	for( size_t i = 0; i < cells.size(); i++ )
	{
		const CellPtr& cell = cells[i];
		applyRaiseCell(bs, cell);
		cell->markDirty();
	}
}

//-----------------------------------//

void TerrainOperation::applyRaiseCell(const BoundingSphere& bs, const CellPtr& cell)
{
	bool updateVB = false;
	
	RenderablePtr rend = cell->getRenderables()[0];
	const VertexBufferPtr& vb = rend->getVertexBuffer();
	std::vector<Vector3>& vertices = vb->getVertices();
	
	for( size_t i = 0; i < vertices.size(); i++ )
	{
		Vector3& v = vertices[i];
		
		if( !bs.intersects(v) )
			continue;

		if( tool == TerrainTool::Raise )
			v.y += brushStrength;
		
		if( tool == TerrainTool::Lower )
			v.y -= brushStrength;

		updateVB = true;
	}

	if( updateVB ) 
		vb->forceRebuild();
}

//-----------------------------------//

static const uint8 AlphaFull = 255;
#define MixAlpha(D, S, A) uint8((D*(1.0f-A) + (S*A)))

static void BlitToImage(Image* brush, Image* source, Image* dest,
						int destX, int destY, int limitX, int limitY)
{
	std::vector<byte>& destBuffer = const_cast<std::vector<byte>&>(dest->getBuffer());
	std::vector<byte>& sourceBuffer = const_cast<std::vector<byte>&>(source->getBuffer());

	uint32 sourceY = limitY;

	for( uint32 y = destY; y < dest->getHeight() && sourceY < source->getHeight(); ++y )
	{
		uint32 sourceX = limitX;

		for( uint32 x = destX; x < dest->getWidth() && sourceX < source->getWidth(); ++x )
		{
			uint32 destPos = (y*dest->getWidth()*4)+(x*4);
			uint32 sourcePos = (sourceY*source->getWidth()*4)+(sourceX*4);

			uint8& dR = destBuffer[destPos+0];
			uint8& dG = destBuffer[destPos+1];
			uint8& dB = destBuffer[destPos+2];
			uint8& dA = destBuffer[destPos+3];

			const uint8& sR = sourceBuffer[sourcePos+0];
			const uint8& sG = sourceBuffer[sourcePos+1];
			const uint8& sB = sourceBuffer[sourcePos+2];
			const uint8& sA = sourceBuffer[sourcePos+3];

			float floatA = sA / AlphaFull;
		
			dR = MixAlpha(dR, sR, floatA);
			dG = MixAlpha(dG, sG, floatA);
			dB = MixAlpha(dB, sB, floatA);
			dA = sA;

			++sourceX;
		}

		++sourceY;
	}
}

//-----------------------------------//

#define WrapFloat(f, n) std::fmod(f, n)

#define WrapVector3(v, n) \
	v.x = WrapFloat(v.x, n); \
	v.y = WrapFloat(v.y, n); \
	v.z = WrapFloat(v.z, n);

void TerrainOperation::applyPaintTool()
{
	if( !paintImage ) return;

	const RenderablePtr& rend = rayQuery.renderable;

	//const Vector3 (&uv)[3] = rayQuery.triangleUV;
	Vector3 uv[3];
	
	uv[0] = rayQuery.triangleUV[0]; WrapVector3(uv[0], 1);
	uv[1] = rayQuery.triangleUV[1]; WrapVector3(uv[1], 1);
	uv[2] = rayQuery.triangleUV[2]; WrapVector3(uv[2], 1);
	
	const Vector3& intUV = rayQuery.intersectionUV;

	float ut = uv[0].x + intUV.x*(uv[1].x-uv[0].x) + intUV.y*(uv[2].x-uv[0].x);
	float vt = uv[0].y + intUV.x*(uv[1].y-uv[0].y) + intUV.y*(uv[2].y-uv[0].y);

	const MaterialPtr& material = rend->getMaterial().Resolve();
	Image* image = material->getTexture(0).Resolve();

	int x = ut * image->getWidth();
	int y = vt * image->getHeight();

	if( tileLock )
	{
		x -= x % paintImage->getWidth();
		x += tileOffsetX;

		y -= y % paintImage->getHeight();
		y += tileOffsetY;
	}
	else
	{
		x -= paintImage->getWidth() / 2;
		y -= paintImage->getWidth() / 2;
	}

	int offsetX = 0;
	int offsetY = 0;

	if( x < 0 )
	{
		offsetX = -x;
		x = 0;
	}

	if( y < 0 )
	{
		offsetY = -y;
		y = 0;
	}

	BlitToImage(brush.get(), image, paintImage.get(), x, y, offsetX, offsetY);

#if PROGRAM_BUFFER
	TexturePtr texture = GetRenderDevice()->getTextureManager()->getTexture(image);
	texture->setImage(image);
#endif
}

//-----------------------------------//

} } // end namespaces

#endif