/************************************************************************
*
* vapor3D Editor © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "PCH.h"
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

	const Vector3& pick = rayQuery.intersection;

	BoundingSphere bs( pick, brushSize );
	
	std::vector<CellPtr> cells;
	getCellsInRange(bs, cells);

	for( uint i = 0; i < cells.size(); i++ )
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

		for( uint i = 0; i < vertices.size(); i++ )
		{
			const Vector3& v = vertices[i];
			heights.push_back( v.y );
		}
	}
	else
	{
		std::vector<Vector3>& vertices = vb->getVertices();

		for( uint i = 0; i < vertices.size(); i++ )
			vertices[i].y = heights[i];

		vb->forceRebuild();
	}
}

//-----------------------------------//

void TerrainOperation::loadSaveImage( std::vector<byte>& state, bool save )
{	
	RenderablePtr rend = rayQuery.renderable;
	MaterialPtr material = rend->getMaterial();
	const TexturePtr& texture = material->getTexture(0);
	const Image* image = texture->getImage();

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

	for( uint i = 0; i < entities.size(); i++ )
	{
		const EntityPtr& node = entities[i];
		const TransformPtr& transform = node->getTransform();
		const BoundingBox& box = transform->getBoundingVolume();

		if( !bs.intersects( BoundingSphere(box) ) )
			continue;

		const CellPtr& cell = node->getComponent<Cell>();
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
	const Vector3& pick = rayQuery.intersection;

	BoundingSphere bs( pick, brushSize );
	
	std::vector<CellPtr> cells;
	getCellsInRange(bs, cells);

	for( uint i = 0; i < cells.size(); i++ )
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
	
	for( uint i = 0; i < vertices.size(); i++ )
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

static void blitToImage(const Image* dest, int destX, int destY,
						const Image* source, int limitX, int limitY)
{
	std::vector<byte>& destBuffer = const_cast<std::vector<byte>&>(dest->getBuffer());
	std::vector<byte>& sourceBuffer = const_cast<std::vector<byte>&>(source->getBuffer());

	int sourceY = limitY;

	for( int y = destY; y < dest->getHeight() && sourceY < source->getHeight(); ++y )
	{
		int sourceX = limitX;

		for( int x = destX; x < dest->getWidth() && sourceX < source->getWidth(); ++x )
		{
			uint destPos = (y*dest->getWidth()*4)+(x*4);
			uint sourcePos = (sourceY*source->getWidth()*4)+(sourceX*4);
		
			destBuffer[destPos+0] = sourceBuffer[sourcePos+0];
			destBuffer[destPos+1] = sourceBuffer[sourcePos+1];
			destBuffer[destPos+2] = sourceBuffer[sourcePos+2];
			destBuffer[destPos+3] = sourceBuffer[sourcePos+3];

			++sourceX;
		}

		++sourceY;
	}
}

//-----------------------------------//

void TerrainOperation::applyPaintTool()
{
	if( !paintImage )
		return;

	const RenderablePtr& rend = rayQuery.renderable;

	const Vector3 (&uv)[3] = rayQuery.triangleUV;
	const Vector3& intUV = rayQuery.intersectionUV;

	float ut = uv[0].x + intUV.x*(uv[1].x-uv[0].x) + intUV.y*(uv[2].x-uv[0].x) + 1;
	float vt = uv[0].y + intUV.x*(uv[1].y-uv[0].y) + intUV.y*(uv[2].y-uv[0].y);

	const MaterialPtr& material = rend->getMaterial();
	const TexturePtr& texture = material->getTexture(0);
	Image* image = (Image*) texture->getImage();

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

	blitToImage(image, x, y, paintImage.get(), offsetX, offsetY);
	texture->setImage(image);
}

//-----------------------------------//

} } // end namespaces