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
#include "Scene/Tags.h"
#include "Render/View.h"

NAMESPACE_ENGINE_BEGIN

//-----------------------------------//

REFLECT_ENUM(PositionMode)
	ENUM(Relative)
	ENUM(Absolute)
REFLECT_ENUM_END()

REFLECT_ENUM(AnchorMode)
	ENUM(TopLeft)
	ENUM(TopCenter)
	ENUM(TopRight)
	ENUM(Right)
	ENUM(BottomRight)
	ENUM(BottomCenter)
	ENUM(BottomLeft)
	ENUM(Left)
	ENUM(Center)
REFLECT_ENUM_END()

REFLECT_CHILD_CLASS(Overlay, Geometry)
	FIELD_ENUM(0, PositionMode, positioning)
	FIELD_ENUM(1, AnchorMode, anchor)
	FIELD_PRIMITIVE(2, Vector3, offset)
	FIELD_PRIMITIVE(3, Vector3, size)
REFLECT_CLASS_END()

//-----------------------------------//

Overlay::Overlay() 
	: positioning( PositionMode::Relative )
	, anchor( AnchorMode::TopLeft )
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
	renderable->onPreRender.Bind(this, &Overlay::onPreRender);

	addRenderable( renderable );
}

//-----------------------------------//

void Overlay::rebuildGeometry()
{
	std::vector<Vector3> pos;
	std::vector< Color > colors;

	pos.push_back( Vector3(borderWidth, borderWidth, -0.1f) );
	pos.push_back( Vector3(borderWidth, size.y-borderWidth, -0.1f) );
	pos.push_back( Vector3(size.x-borderWidth, size.y-borderWidth, -0.1f) );
	pos.push_back( Vector3(size.x-borderWidth, borderWidth, -0.1f) );

	Color color = backgroundColor;
	color.a = opacity;
	
	colors.push_back(color);
	colors.push_back(color);
	colors.push_back(color);
	colors.push_back(color);

	if( borderWidth > 0 )
	{
		pos.push_back( Vector3(0, 0, -0.2f) );
		pos.push_back( Vector3(0, size.y, -0.2f) );
		pos.push_back( Vector3(size.x, size.y, -0.2f) );
		pos.push_back( Vector3(size.x, 0, -0.2f) );
	
		color = borderColor;

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

	updateBounds();
}

//-----------------------------------//

void Overlay::update( float )
{
	rebuildGeometry();
}

//-----------------------------------//

void Overlay::layout(const Vector2i& screen)
{
	if(positioning == PositionMode::Absolute)
	{
		position.x = offset.x;
		position.y = offset.y;
		return;
	}

	// Use the world bounding volume to take scale into account.
	const BoundingBox& box = getWorldBoundingVolume();
	Vector3 bounds = box.max - box.min;

	switch(anchor)
	{
	case AnchorMode::TopLeft:
		position.x = 0;
		position.y = 0;
		break;
	case AnchorMode::TopCenter:
		position.x = screen.x / 2.0f;
		position.y = 0;
		break;
	case AnchorMode::TopRight:
		position.x = screen.x - bounds.x;
		position.y = 0;
		break;
	case AnchorMode::Right:
		position.x = 0;
		position.y = screen.y;
		break;
	case AnchorMode::BottomRight:
		position.x = 0;
		position.y = screen.y;
		break;
	case AnchorMode::BottomCenter:
		position.x = 0;
		position.y = screen.y;
		break;
	case AnchorMode::BottomLeft:
		position.x = 0;
		position.y = screen.y;
		break;
	case AnchorMode::Left:
		position.x = 0;
		position.y = screen.y;
		break;
	case AnchorMode::Center:
		position.x = 0;
		position.y = screen.y;
		break;
	}

	position.x += offset.x;
	position.y += offset.y;
}

//-----------------------------------//

void Overlay::onPreRender(RenderView* view, const RenderState& state)
{
	// Recalculate the layout for this view.
	layout( view->getSize() );

	// Update the position on the render state.
	RenderState& mstate = const_cast<RenderState&>(state);
	mstate.modelMatrix.tx = position.x;
	mstate.modelMatrix.ty = position.y;
}

//-----------------------------------//

void Overlay::setOffset( int x, int y )
{
	offset.x = x;
	offset.y = y;
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