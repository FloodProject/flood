/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "vapor/PCH.h"
#include "vapor/gui/Overlay.h"
#include "vapor/scene/Entity.h"
#include "vapor/scene/Transform.h"

namespace vapor {

//-----------------------------------//

BEGIN_CLASS_PARENT(Overlay, Geometry)
END_CLASS()

//-----------------------------------//

Overlay::Overlay() 
	: positioning( PositionMode::Relative )
	, anchor( Anchor::TopLeft )
	, opacity(1.0f)
{
	createGeometry();
}

//-----------------------------------//

void Overlay::createGeometry()
{
	VertexBufferPtr vb( new VertexBuffer() );
	material = new Material("OverlayMaterial");
	
	renderable = new Renderable( PolygonType::Quads );
	renderable->setVertexBuffer(vb);
	renderable->setMaterial(material);

	// Add a new renderable to hold the text geometry
	addRenderable( renderable, RenderStage::Overlays );
}

//-----------------------------------//

void Overlay::rebuildGeometry()
{
	const VertexBufferPtr& vb = renderable->getVertexBuffer();

	// Position.
	std::vector<Vector3> pos;

	pos.push_back( Vector3::Zero );
	pos.push_back( Vector2i(0, size.y) );
	pos.push_back( size );
	pos.push_back( Vector2i(size.x, 0) );

	Color color = Color(1.0f, 1.0f, 1.0f, opacity);

	std::vector< Color > colors;
	colors.push_back(color);
	colors.push_back(color);
	colors.push_back(color);
	colors.push_back(color);

	vb->set( VertexAttribute::Position, pos );
	vb->set( VertexAttribute::Color, colors );
}

//-----------------------------------//

void Overlay::update( double )
{
	EntityPtr entity = getEntity();
	TransformPtr transform = entity->getTransform();
	
	//transform->reset();

	//Vector3 move;

	//switch(anchor)
	//{
	//case Anchor::TopLeft:
	//	#pragma TODO("Implement overlay positioning")
	//	break;
	//}

	transform->setPosition(position);

	rebuildGeometry();
}

//-----------------------------------//

void Overlay::setPosition( int x, int y )
{
	position.x = x;
	position.y = y;
}

//-----------------------------------//

void Overlay::setOpacity( float opacity )
{
	this->opacity = opacity;

	MaterialPtr material = getRenderables()[0]->getMaterial();

	if(!material)
		return;

	material->setBlending(BlendSource::SourceAlpha, BlendDestination::InverseSourceAlpha);

	rebuildGeometry();
}

//-----------------------------------//

} // end namespace