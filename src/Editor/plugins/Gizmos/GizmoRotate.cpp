/************************************************************************
*
* vapor3D Engine © (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Editor/API.h"
#include "GizmoRotate.h"
#include "Geometry/Cube.h"

NAMESPACE_EDITOR_BEGIN

//-----------------------------------//

GizmoRotate::GizmoRotate( const EntityPtr& entity, const CameraWeakPtr& camera )
	: Gizmo(entity, camera)
{ }

//-----------------------------------//

void GizmoRotate::buildGeometry()
{
	//material->setLineWidth(3.0f);

	lines = generateCircles();

	RenderablePtr renderable = AllocateHeap(Renderable);
	renderable->setPrimitiveType(PrimitiveType::Lines);
	renderable->setGeometryBuffer(lines);
	renderable->setMaterial(material);
	renderable->setRenderLayer(RenderLayer::PostTransparency);
	renderable->setRenderPriority(100);

	addRenderable(renderable);
}

//-----------------------------------//

static void TransformVertices(std::vector<Vector3>& pos,
					   std::vector<Vector3>& vs, Matrix4x3& transform)
{
	for( size_t i = 0; i < vs.size(); i++ )
	{
		const Vector3& v = vs[i];
		pos.push_back( transform*v );
	}
}

static const byte SLICES = 32;

GeometryBufferPtr GizmoRotate::generateCircles()
{
	// Vertex data
	std::vector< Vector3 > pos;
	std::vector< Vector3 > colors;

	Matrix4x3 transform;

	// X axis
	std::vector< Vector3 > posX;
	generateCircle(posX, SLICES);
	generateColors(colors, X);

	transform = Matrix4x3::createScale( Vector3(0.4f) );
	TransformVertices(pos, posX, transform);

	// Y axis
	std::vector< Vector3 > posY;
	generateCircle(posY, SLICES);
	generateColors(colors, Y);
	
	transform = Matrix4x3::createScale( Vector3(0.4f) );
	transform = transform*Matrix4x3::createRotation( EulerAngles(0, 90, 0) );
	TransformVertices(pos, posY, transform);
	
	// Z axis
	std::vector< Vector3 > posZ;
	generateCircle(posZ, SLICES);
	generateColors(colors, Z);

	transform = Matrix4x3::createScale( Vector3(0.4f) );
	transform = transform*Matrix4x3::createRotation( EulerAngles(90, 0, 0) );
	TransformVertices(pos, posZ, transform);

	// Translate it a bit.
	transform = Matrix4x3::createTranslation( Vector3::UnitY * 0.5f );
	for( uint i = 0; i < pos.size(); i++ )
	{
		Vector3& v = pos[i];
		v = transform*v;
	}

	// Vertex buffer setup
	GeometryBufferPtr gb = AllocateHeap(GeometryBuffer);

	assert( pos.size() == colors.size() );

	gb->set( VertexAttribute::Position, pos );
	gb->set( VertexAttribute::Color, colors );
	
	return gb;
}

//-----------------------------------//

void GizmoRotate::highlightAxis( GizmoAxis::Enum axis, bool highlight )
{
	Color c = (highlight) ? Color::White : getAxisColor(axis);

	for( int i = SLICES*2*axis; i < SLICES*2*(axis+1); i++ )
	{
		Vector3* color = (Vector3*) lines->getAttribute( VertexAttribute::Color, i );
		*color = c;
	}

	lines->forceRebuild();
}

//-----------------------------------//

void GizmoRotate::generateCircle(std::vector<Vector3>& pos, int slices)
{
	float phi = PI / 2.0f;
	float delta = (PI * 2.0f) / (float) slices;

	for( int i = 0; i <= slices; i++ )
	{
		float theta = i * delta;
		
		float x = sin(phi)*cos(theta);
		float y = sin(phi)*sin(theta);
		float z = cos(phi);

		Vector3 v(x, y, z);
		pos.push_back(v);

		if( i > 0 && i < slices )
			pos.push_back(v);	
	}
}

//-----------------------------------//

void GizmoRotate::generateColors( std::vector<Vector3>& colors, const Color& c )
{
	for( int i = 0; i <= SLICES; i++ )
	{
		colors.push_back(c);

		if( i > 0 && i < SLICES )
			colors.push_back(c);
	}
}

//-----------------------------------//

NAMESPACE_EDITOR_END