/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "vapor/PCH.h"
#include "gui/Overlay.h"
#include "scene/Entity.h"
#include "scene/Transform.h"

namespace vapor {

//-----------------------------------//

BEGIN_CLASS_PARENT(Overlay, Geometry)
END_CLASS()

//-----------------------------------//

Overlay::Overlay() 
	: positioning( PositionMode::Relative )
	, anchor( Anchor::TopLeft )
	, opacity(1.0f)
	, borderWidth(0)
	, borderColor(Color::Black)
	, backgroundColor(Color::White)
{
	createGeometry();
}

//-----------------------------------//

void Overlay::createGeometry()
{
	material = new Material("OverlayMaterial");

	renderable = new Renderable( PolygonType::Quads );
	renderable->setVertexBuffer( new VertexBuffer() );
	renderable->setMaterial(material);
	renderable->setRenderLayer(RenderLayer::Overlays);

	addRenderable( renderable );

	MaterialPtr borderMaterial = new Material("OverlayBorderMaterial");
	borderMaterial->setDepthTest(false);

	borderRenderable = new Renderable( PolygonType::LineLoop );
	borderRenderable->setVertexBuffer( new VertexBuffer() );
	borderRenderable->setMaterial( borderMaterial  );
	borderRenderable->setRenderLayer(RenderLayer::Overlays);
	borderRenderable->setRenderPriority(10);

	addRenderable( borderRenderable );
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

	Color color = backgroundColor;
	color.a = opacity;

	std::vector< Color > colors;
	colors.push_back(color);
	colors.push_back(color);
	colors.push_back(color);
	colors.push_back(color);

	vb->set( VertexAttribute::Position, pos );
	vb->set( VertexAttribute::Color, colors );

	rebuildBorderGeometry();
}

//-----------------------------------//

void Overlay::rebuildBorderGeometry()
{
	const VertexBufferPtr& vb = borderRenderable->getVertexBuffer();

	if( borderWidth <= 0 )
	{
		vb->clear();
		return;
	}

	std::vector<Vector3> pos;

	pos.push_back( Vector3::Zero );
	pos.push_back( Vector2i(0, size.y) );
	pos.push_back( size );
	pos.push_back( Vector2i(size.x, 0) );

	const Color& color = borderColor;

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
	const TransformPtr& transform = entity->getTransform();
	
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