/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Engine/API.h"
#include "GUI/Overlay.h"
#include "Scene/Entity.h"
#include "Scene/Transform.h"

namespace vapor {

//-----------------------------------//

REFLECT_CHILD_CLASS(Overlay, Geometry)
REFLECT_CLASS_END()

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
	material = MaterialCreate( AllocatorGetHeap(), "OverlayMaterial");

	renderable = Allocate(Renderable, AllocatorGetHeap(), PolygonType::Quads);
	renderable->setVertexBuffer( Allocate(VertexBuffer, AllocatorGetHeap()) );
	renderable->setMaterial(material);
	renderable->setRenderLayer(RenderLayer::Overlays);

	addRenderable( renderable );

	MaterialHandle borderHandle = MaterialCreate( AllocatorGetHeap(), "OverlayBorderMaterial");
	
	Material* borderMaterial = borderHandle.Resolve();
	borderMaterial->setDepthTest(false);

	borderRenderable = Allocate(Renderable, AllocatorGetHeap(), PolygonType::LineLoop);
	borderRenderable->setVertexBuffer( Allocate(VertexBuffer, AllocatorGetHeap()) );
	borderRenderable->setMaterial( borderHandle );
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
	pos.push_back( Vector2(0, size.y) );
	pos.push_back( size );
	pos.push_back( Vector2(size.x, 0) );

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
	pos.push_back( Vector2(0, size.y) );
	pos.push_back( size );
	pos.push_back( Vector2(size.x, 0) );

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

void Overlay::update( float )
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

	if( renderables.empty() ) return;

	MaterialHandle materialHandle = renderables[0]->getMaterial();

	Material* material = materialHandle.Resolve();
	if( !material ) return;

	material->setBlending(BlendSource::SourceAlpha, BlendDestination::InverseSourceAlpha);

	rebuildGeometry();
}

//-----------------------------------//

} // end namespace