/************************************************************************
*
* vapor3D Editor © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "PCH.h"
#include "TerrainPlugin.h"
#include "TerrainPage.h"
#include "Editor.h"
#include "UndoManager.h"
#include "EditorIcons.h"
#include "Viewframe.h"

namespace vapor { namespace editor {

//-----------------------------------//

BEGIN_EVENT_TABLE(TerrainPlugin, wxEvtHandler)
	EVT_TIMER(wxID_ANY, TerrainPlugin::onTimer)
END_EVENT_TABLE()

//-----------------------------------//

TerrainPlugin::TerrainPlugin( EditorFrame* frame )
	: Plugin(frame)
	, terrainPage(nullptr)
	, timer(this)
	, terrainOperation(nullptr)
{ }

//-----------------------------------//

PluginMetadata TerrainPlugin::getMetadata()
{
	PluginMetadata metadata;
	
	metadata.name = "Terrains";
	metadata.description = "Provides terrain editing tools.";
	metadata.author = "triton";
	metadata.version = "1.0";

	return metadata;
}

//-----------------------------------//

void TerrainPlugin::onPluginEnable()
{
	wxToolBar* toolBar = editor->getToolbar();
	
	buttonRaise = toolBar->AddTool( TerrainTool::Raise/*wxID_ANY*/, "Raise/Lower",
		wxMEMORY_BITMAP(terrain_raise_lower), "Raises/Lowers the terrain",
		wxITEM_RADIO );
	addTool(buttonRaise);

	buttonPaint = toolBar->AddTool( TerrainTool::Paint/*wxID_ANY*/, "Paint",
		wxMEMORY_BITMAP(terrain_paint), "Paints the terrain", wxITEM_RADIO );
	addTool(buttonPaint);

	wxNotebook* notebookCtrl = editor->getNotebook();
	wxImageList* imageList = notebookCtrl->GetImageList();

	wxBitmap iconWorld = wxMEMORY_BITMAP(world);
	int iconTerrain = imageList->Add(iconWorld);

	terrainPage = new TerrainPage( engine, notebookCtrl );
	notebookCtrl->AddPage( terrainPage, /*"Terrains"*/"", false, iconTerrain );

	toolBar->Bind( wxEVT_COMMAND_TOOL_CLICKED,
		&TerrainPlugin::onToolClick, this,
		buttonRaise->GetId(), buttonPaint->GetId() );
}

//-----------------------------------//

void TerrainPlugin::onToolClick(wxCommandEvent& event)
{
	tool = (TerrainTool::Enum) event.GetId();
	
	delete terrainOperation;
	terrainOperation = nullptr;
}

//-----------------------------------//

void TerrainPlugin::onPluginDisable()
{
	removePage( terrainPage );
}

//-----------------------------------//

void TerrainPlugin::onNodeSelect( const NodePtr& node )
{
	if( !node->getInstanceType().is<Terrain>() )
		return;

	terrain = std::static_pointer_cast<Terrain>(node);
}

//-----------------------------------//

void TerrainPlugin::onNodeUnselect( const NodePtr& )
{

}

//-----------------------------------//

void TerrainPlugin::onTimer( wxTimerEvent& )
{
	Mouse* mouse = engine->getInputManager()->getMouse();
	const MouseInfo& info = mouse->getMouseInfo();

	if( info.leftButton )
	{
		assert( terrainOperation != nullptr );

		terrainOperation->applyTool();
		editor->redrawView();
	}
}

//-----------------------------------//

void TerrainPlugin::onMouseButtonPress( const MouseButtonEvent& mbe )
{
	if( mbe.isLeftButton() )
	{
		setupOperation(mbe);
	}
	else if( mbe.isRightButton() )
	{
		createContextMenu(mbe);
	}
}

//-----------------------------------//

void TerrainPlugin::onMouseButtonRelease( const MouseButtonEvent& )
{
	registerUndoOperation();
}

//-----------------------------------//

void TerrainPlugin::onMouseDrag( const MouseDragEvent& mde )
{
	//if( !mde.info.leftButton )
	//	return;

	MouseButtonEvent mbe( MouseEventType::MousePress );
	mbe.x = mde.x;
	mbe.y = mde.y;
	mbe.button = MouseButton::Left;

	// We need to handle the case where the user clicks but doesn't move
	// the mouse anymore. As we don't receive further events in this case,
	// we check if the mouse is still pressed.

	setupOperation(mbe);
}

//-----------------------------------//

void TerrainPlugin::onMouseLeave()
{
	// We use this event to check if the user leaves the window while
	// dragging in the middle of a terrain operation. If that isn't
	// the case, then we've got nothing to do here.
	if( !terrainOperation )
		return;

	registerUndoOperation();
}

//-----------------------------------//

bool TerrainPlugin::pickCell( const MouseButtonEvent& mbe )
{
	if( !terrain )
		return false;

	RenderView* view = viewframe->getView();
	const CameraPtr& camera = view->getCamera(); 
	
	// Get a ray given the screen location clicked.
	const Ray& pickRay = camera->getRay( mbe.x, mbe.y );

	Plane ground( Vector3::UnitY, 0 );
	
	float distance;
	if( !ground.intersects(pickRay, distance) )
		return false;
		
	Vector3 pt = pickRay.getPoint(distance);
	coords = terrain->getCoords(pt);
		
	return true;
}

//-----------------------------------//

void TerrainPlugin::createContextMenu( const MouseButtonEvent& mbe )
{
	std::string menuTitle( "Terrain Operations" );
	wxMenu menu( menuTitle );

	if( pickCell(mbe) )
	{
		CellPtr cell = terrain->getCell(coords.x, coords.y);

		std::string newTitle = String::format( "%s (Cell %hd,%hd)",
			menuTitle.c_str(), coords.x, coords.y );

		menu.SetTitle( newTitle );

		if( cell )
		{
			wxMenuItem* rebuildNormalsButton =
				menu.Append(wxID_ANY, "Rebuild normals");

			menu.Bind( wxEVT_COMMAND_MENU_SELECTED,
				&TerrainPlugin::onRebuildNormals, this,
				rebuildNormalsButton->GetId() );

			// TODO:
			//wxMenuItem* removeCellButton =
			//	menu.Append(wxID_ANY, "Remove cell");

			//menu.Bind( wxEVT_COMMAND_MENU_SELECTED,
			//	&TerrainPlugin::onRemoveCell, this,
			//	removeCellButton->GetId() );
		}
		else
		{
			wxMenuItem* createCellButton =
				menu.Append(wxID_ANY, "Create cell");

			menu.Bind( wxEVT_COMMAND_MENU_SELECTED,
				&TerrainPlugin::onCreateCell, this,
				createCellButton->GetId() );
		}
	}
	else
	{
		if( !terrain )
			menu.Append(wxID_ANY, "Invalid terrain" );
		else
			menu.Append(wxID_ANY, "Invalid cell" );
	}

	wxPoint clientpt( mbe.x, mbe.y );
	editor->PopupMenu(&menu, clientpt);
}

//-----------------------------------//

void TerrainPlugin::onCreateCell( wxCommandEvent& event )
{
	terrain->addCell(coords.x, coords.y);

	editor->redrawView();
}

//-----------------------------------//

void TerrainPlugin::onRebuildNormals( wxCommandEvent& event )
{
	CellPtr cell = terrain->getCell(coords.x, coords.y);
	assert( cell != nullptr );

	cell->rebuildNormals();

	editor->redrawView();
}

//-----------------------------------//

void TerrainPlugin::createOperation( const RayTriangleQueryResult& res )
{
	if( terrainOperation )
		return;

	// If the left Shift is held down, then lower.
	Keyboard* keyboard = engine->getInputManager()->getKeyboard();
	bool raise = !keyboard->isKeyPressed( Keys::LShift );	

	terrainOperation = new TerrainOperation( tool, res );
	terrainOperation->terrain = terrain;

	terrainOperation->brushSize = terrainPage->getBrushSize();
	terrainOperation->brushStrength = terrainPage->getBrushStrength();
	terrainOperation->paintImage = terrainPage->getPaintImage();

	terrainOperation->tileLock = terrainPage->getTileLock();
	terrainOperation->tileOffsetX = terrainPage->getTileOffsetX();
	terrainOperation->tileOffsetY = terrainPage->getTileOffsetY();
}

//-----------------------------------//

void TerrainPlugin::registerUndoOperation()
{
	if( !terrainOperation )
		return;

	timer.Stop();

	// Register the operation in the stack so it can be undone later.
	terrainOperation->ready();
	
	UndoManager* undoManager = editor->getUndoManager();
	undoManager->registerOperation( terrainOperation );

	terrainOperation = nullptr;
}

//-----------------------------------//

void TerrainPlugin::setupOperation( const MouseButtonEvent& mb )
{
	const int TERRAIN_TIMER_MS = 500;

	RayTriangleQueryResult res;
	
	if( !pickTerrain(mb, res) )
		return;

	if( !terrainOperation )
		createOperation(res);
	else
		terrainOperation->rayQuery = res;

	terrainOperation->applyTool();
	editor->redrawView();

	timer.Start( TERRAIN_TIMER_MS );
}

//-----------------------------------//

bool TerrainPlugin::pickTerrain( const MouseButtonEvent& mb, RayTriangleQueryResult& res )
{
	const ScenePtr& scene = engine->getSceneManager();

	RenderView* view = viewframe->getView();
	const CameraPtr& camera = view->getCamera(); 
	
	// Get a ray given the screen location clicked.
	const Ray& pickRay = camera->getRay( mb.x, mb.y );

	RayBoxQueryResult query;
	
	if( !scene->doRayBoxQuery(pickRay, query) )
		return false;

	const NodePtr& node = query.node;
	
	if( !node ) 
		return false;

	if( !node->getParent()->getInstanceType().is<Terrain>() )
		return false;

	if( !scene->doRayTriangleQuery(pickRay, res, node) )
		return false; // Query does not intersect the terrain.

	return true;
}

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

	foreach(const CellPtr& cell, cells) 
	{
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

		foreach( const Vector3& v, vb->getVertices() )
			heights.push_back( v.y );
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
	foreach(const NodePtr& node, terrain->getNodes() ) 
	{
		const TransformPtr& transform = node->getTransform();
		const BoundingBox& box = transform->getBoundingVolume();

		if( !bs.intersects( BoundingSphere(box) ) )
			continue;
		
		GeometryPtr geometry = node->getComponent<Geometry>();
		assert( geometry != nullptr );

		const RenderableList& renderables = geometry->getRenderables();
		assert( !renderables.empty() );

		const CellPtr& cell = boost::static_pointer_cast<Cell>(renderables[0]);
		cells.push_back(cell);
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

	foreach(const CellPtr& cell, cells) 
	{
		applyRaiseCell(bs, cell);
	}
}

//-----------------------------------//

void TerrainOperation::applyRaiseCell(const BoundingSphere& bs, const CellPtr& rend)
{
	bool updateVB = false;
	VertexBufferPtr vb = rend->getVertexBuffer();

	foreach( Vector3& v, vb->getVertices() )
	{
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

	uint sourceY = limitY;

	for( uint y = destY; y < dest->getHeight() && sourceY < source->getHeight(); ++y )
	{
		uint sourceX = limitX;

		for( uint x = destX; x < dest->getWidth() && sourceX < source->getWidth(); ++x )
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

#define uv rayQuery.triangleUV
#define intUV rayQuery.intersectionUV

void TerrainOperation::applyPaintTool()
{
	RenderablePtr rend = rayQuery.renderable;

	float ut = uv[0].x + intUV.x*(uv[1].x-uv[0].x) + intUV.y*(uv[2].x-uv[0].x) + 1;
	float vt = uv[0].y + intUV.x*(uv[1].y-uv[0].y) + intUV.y*(uv[2].y-uv[0].y);

	if( !paintImage )
		return;

	MaterialPtr material = rend->getMaterial();
	const TexturePtr& texture = material->getTexture(0);
	const Image* image = texture->getImage();

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
	texture->setImage((Image*)image);
}

//-----------------------------------//

} } // end namespaces