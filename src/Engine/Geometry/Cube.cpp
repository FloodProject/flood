/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#include "Engine/API.h"
#include "Engine/Geometry/Cube.h"
#include "Graphics/RenderBatch.h"

NAMESPACE_ENGINE_BEGIN

//-----------------------------------//

REFLECT_CHILD_CLASS(Cube, Geometry)
REFLECT_CLASS_END()

//-----------------------------------//

Cube::Cube()
    : width(1.0f), height(1.0f)
{
    create();
}

//-----------------------------------//

Cube::Cube( float width, float height )
    : width(width), height(height)
{
    create();
}

//-----------------------------------//

void Cube::create()
{
    GeometryBufferPtr gb = AllocateThis(GeometryBuffer);
    
    MaterialHandle mat = MaterialCreate(AllocatorGetHeap(), "Cube");
    mat.Resolve()->setShader("VertexColor");

    RenderablePtr rend = RenderBatchCreate( AllocatorGetHeap() );
    rend->setPrimitiveType(PrimitiveType::Quads);
    rend->setGeometryBuffer(gb);
    rend->setMaterial(mat);

    BuildCube(gb.get(), width, height);

    addRenderable(rend);
}

//-----------------------------------//

#define ADD_BOX_FACE( a, b, c, d )              \
    pos.Push( a*width );                   \
    pos.Push( b*width );                   \
    pos.Push( c*width );                   \
    pos.Push( d*width );

#define v(a,b,c) Vector3(a,b,c)

void BuildCube( GeometryBuffer* gb, float width, float height )
{
    // Vertex position data
    Vector<Vector3> pos;
    pos.Reserve(24);

    ADD_BOX_FACE( v( 1, 1, 1), v(-1, 1, 1), v(-1,-1, 1), v( 1,-1, 1) )  // Front
    ADD_BOX_FACE( v( 1,-1, 1), v(-1,-1, 1), v(-1,-1,-1), v( 1,-1,-1) )  // Bottom
    ADD_BOX_FACE( v( 1,-1,-1), v(-1,-1,-1), v(-1, 1,-1), v( 1, 1,-1) )  // Back
    ADD_BOX_FACE( v( 1, 1,-1), v(-1, 1,-1), v(-1, 1, 1), v( 1, 1, 1) )  // Top
    ADD_BOX_FACE( v(-1, 1, 1), v(-1, 1,-1), v(-1,-1,-1), v(-1,-1, 1) )  // Left
    ADD_BOX_FACE( v( 1, 1,-1), v( 1, 1, 1), v( 1,-1, 1), v( 1,-1,-1) )  // Right

    // Vertex color data
    Vector<Vector3> colors( pos.Size(), Color::White );

    // Vertex tex coords data
    Vector< Vector3 > coords( pos.Size(), Vector3::Zero );
    
    // Top
    coords.Push( Vector2(0.0f, 1.0f) );
    coords.Push( Vector2(0.0f, 0.0f) );
    coords.Push( Vector2(1.0f, 0.0f) );
    coords.Push( Vector2(1.0f, 1.0f) );
    // Bottom
    coords.Push( Vector2(1.0f, 1.0f) );
    coords.Push( Vector2(0.0f, 1.0f) );
    coords.Push( Vector2(0.0f, 0.0f) );
    coords.Push( Vector2(1.0f, 0.0f) );
    // Front
    coords.Push( Vector2(0.0f, 0.0f) );
    coords.Push( Vector2(1.0f, 0.0f) );
    coords.Push( Vector2(1.0f, 1.0f) );
    coords.Push( Vector2(0.0f, 1.0f) );
    // Back
    coords.Push( Vector2(1.0f, 0.0f) );
    coords.Push( Vector2(1.0f, 1.0f) );
    coords.Push( Vector2(0.0f, 1.0f) );
    coords.Push( Vector2(0.0f, 0.0f) );
    // Left
    coords.Push( Vector2(0.0f, 0.0f) );
    coords.Push( Vector2(1.0f, 0.0f) );
    coords.Push( Vector2(1.0f, 1.0f) );
    coords.Push( Vector2(0.0f, 1.0f) );
    // Right
    coords.Push( Vector2(0.0f, 0.0f) );
    coords.Push( Vector2(1.0f, 0.0f) );
    coords.Push( Vector2(1.0f, 1.0f) );
    coords.Push( Vector2(0.0f, 1.0f) );

    // Vertex buffer setup
    gb->set( VertexAttribute::Position, pos );
    gb->set( VertexAttribute::Color, colors );
    gb->set( VertexAttribute::TexCoord0, coords );
}

//-----------------------------------//

NAMESPACE_ENGINE_END