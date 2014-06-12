/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#include "Engine/API.h"
#include "Engine/Scene/Grid.h"

NAMESPACE_ENGINE_BEGIN

//-----------------------------------//

REFLECT_CHILD_CLASS(Grid, Geometry)
REFLECT_CLASS_END()

static const Vector3 LineColor(0.4f);
static const Vector3 MainLineColor(0.9f);
static const short MainLineStep = 8;

//-----------------------------------//

Grid::Grid()
    : sizeX(1024)
    , sizeZ(1024)
    , divX(32)
    , divZ(32)
    , strongMainLines(true)
{
}

//-----------------------------------//

GeometryBufferPtr Grid::buildGeometry()
{
    // Create a new VBO and upload triangle data
    GeometryBuffer* gb = AllocateThis(GeometryBuffer);

    // Vertex data
    Vector< Vector3 > vertex;
    Vector< Vector3 > colors;
    
    // Let's make the lines perpendicular to the X-axis.
    float x_pos = -sizeX / 2;
    float z_pos = -sizeZ / 2;
    
    for( int i = 0; i < divX+1; i++ )
    {
        vertex.Push( Vector3(x_pos, 0.0f, z_pos) );
        vertex.Push( Vector3(-x_pos, 0.0f, z_pos) );

        bool isMainLine = (i % MainLineStep == 0) && (i != 0) && (i != divX);

        if( strongMainLines && isMainLine )
        {
            colors.Push( MainLineColor );
            colors.Push( MainLineColor );
        }
        else
        {
            colors.Push( LineColor );
            colors.Push( LineColor );
        }

        z_pos += sizeZ / divZ;
    }

    // Now the lines perpendicular to the Z-axis.
    x_pos = -sizeX / 2;
    z_pos = -sizeZ / 2;
    
    for( int i = 0; i < divZ+1; i++ )
    {
        vertex.Push( Vector3( x_pos, 0.0f, z_pos ) );
        vertex.Push( Vector3( x_pos, 0.0f, -z_pos ) );

        bool isMainLine = (i % MainLineStep == 0) && (i != 0) && (i != divX);

        if( strongMainLines && isMainLine )
        {
            colors.Push( MainLineColor );
            colors.Push( MainLineColor );
        }
        else
        {
            colors.Push( LineColor );
            colors.Push( LineColor );
        }

        x_pos += sizeX / divX;
    }

    // Vertex buffer setup
    gb->set( VertexAttribute::Position, vertex );
    gb->set( VertexAttribute::Color, colors );

    return gb;
}

//-----------------------------------//

void Grid::update( float update )
{
    if( !renderables.Empty() ) return;

    MaterialHandle materialHandle = MaterialCreate(AllocatorGetHeap(), "Grid");

    RenderBatch* rend = AllocateHeap(Renderable);
    rend->setPrimitiveType(PrimitiveType::Lines);
    rend->setGeometryBuffer( buildGeometry() );
    rend->setMaterial( materialHandle );

    addRenderable( rend );
}

//-----------------------------------//

NAMESPACE_ENGINE_END