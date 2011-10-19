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

NAMESPACE_ENGINE_BEGIN

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
	material = MaterialCreate(AllocatorGetThis(), "OverlayMaterial");

	renderable = AllocateThis(Renderable);
	renderable->setPrimitiveType(PolygonType::Quads);
	renderable->setGeometryBuffer( AllocateThis(GeometryBuffer) );
	renderable->setMaterial(material);
	renderable->setRenderLayer(RenderLayer::Overlays);

	addRenderable( renderable );
}

//-----------------------------------//

void Overlay::rebuildGeometry()
{
	std::vector<Vector3> pos;
	std::vector< Color > colors;

	pos.push_back( Vector3(borderWidth, borderWidth, -1) );
	pos.push_back( Vector3(borderWidth, size.y-borderWidth, -1) );
	pos.push_back( Vector3(size.x-borderWidth, size.y-borderWidth, -1) );
	pos.push_back( Vector3(size.x-borderWidth, borderWidth, -1) );

	Color color = backgroundColor;
	color.a = opacity;
	
	colors.push_back(color);
	colors.push_back(color);
	colors.push_back(color);
	colors.push_back(color);

	if( borderWidth > 0 )
	{
		pos.push_back( Vector3(0, 0, -2) );
		pos.push_back( Vector3(0, size.y, -2) );
		pos.push_back( Vector3(size.x, size.y, -2) );
		pos.push_back( Vector3(size.x, 0, -2) );
	
		color = borderColor;
		//color.a = opacity;

		colors.push_back(color);
		colors.push_back(color);
		colors.push_back(color);
		colors.push_back(color);
	}

	const GeometryBufferPtr& gb = renderable->getGeometryBuffer();
	
	gb->clear();
	gb->declarations.reset();

	gb->set( VertexAttribute::Position, pos );
	gb->set( VertexAttribute::Color, colors );
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
}

//-----------------------------------//

NAMESPACE_ENGINE_END