/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#include "Engine/API.h"
#include "Engine/Terrain/Cell.h"
#include "Engine/Terrain/Terrain.h"
#include "Engine/Scene/Entity.h"
#include "Engine/Scene/Geometry.h"
#include "Core/Math/Helpers.h"
#include "Core/Math/Color.h"

NAMESPACE_ENGINE_BEGIN

//-----------------------------------//

REFLECT_CHILD_CLASS(Cell, Geometry)
    FIELD_CLASS_PTR(5, Image, ImageHandle, image, Handle)
    FIELD_CLASS_PTR(6, Material, MaterialHandle, material, Handle)
REFLECT_CLASS_END()

//-----------------------------------//

Cell::Cell() { }

//-----------------------------------//

Cell::Cell( int32 x, int32 y )
    : x(x), y(y)
{ }

//-----------------------------------//

void Cell::setSettings( const TerrainSettings& settings )
{
    this->settings = &settings;

    int32 size = settings.TextureSize;
    
    image = ImageCreate(AllocatorGetHeap(), size, size, PixelFormat::R8G8B8A8);
    image.Resolve()->setColor( Color::LightGrey );

    // Make a copy of the default cell material.
    Material* mat = AllocateHeap(Material, *settings.Material.Resolve());
    mat->setTexture(0, image);

    material = HandleCast<Material>( ResourceHandleCreate(mat) );
}

//-----------------------------------//

void Cell::setHeights( const Vector<float>& heights )
{
    this->heights = heights;

    rend = AllocateThis(Renderable);
    rend->setPrimitiveType(PrimitiveType::Triangles);
    rend->setGeometryBuffer( AllocateThis(GeometryBuffer) );
    rend->setMaterial(material);

    addRenderable(rend);

    rebuildGeometry();
    rebuildNormals();
}

//-----------------------------------//

void Cell::rebuildGeometry()
{
    LogInfo( "Rebuilding geometry of cell (%hd, %hd)", x, y );

    rebuildVertices();
    rebuildIndices();

    // Force bounding-box update.
    rebuildBoundingBox();
}

//-----------------------------------//

void Cell::rebuildVertices()
{
    if( heights.Empty() ) return;

    // Vertex data
    Vector<Vector3> vertex;
    Vector<Vector3> texCoords;

    int numTiles = settings->NumberTiles;
    int sizeCell = settings->CellSize;

    float offsetX = float(x * sizeCell);
    float offsetZ = float(y * sizeCell);

    float tileSize = float(sizeCell / numTiles);
    uint numExpectedVertices = (numTiles+1)*(numTiles+1);

    for( size_t i = 0; i < numExpectedVertices; i++ )
    {
        uint row = i % (numTiles+1);
        uint col = i / (numTiles+1);

        float X = offsetX + tileSize*row;
        float Z = offsetZ + tileSize*col;
        float Y = heights[i] * settings->MaxHeight;
        
        vertex.Push( Vector3(X, Y, Z) );
        texCoords.Push( Vector2(X/sizeCell, Z/sizeCell) );
    }

    assert( vertex.Size() == numExpectedVertices );
    assert( texCoords.Size() == numExpectedVertices );

    // Vertex buffer setup.
    const GeometryBufferPtr& gb = rend->getGeometryBuffer();
    gb->set( VertexAttribute::Position, vertex );
    gb->set( VertexAttribute::TexCoord0, texCoords );
}

//-----------------------------------//

void Cell::rebuildIndices()
{
    // Index data.
    Vector<uint16> indices;

    const uint32 numTiles = settings->NumberTiles;
    
    for( size_t col = 0; col < numTiles; col++ )
    {
        for( size_t row = 0; row < numTiles; row++ )
        {
            int i = col * (numTiles + 1) + row;

            // First triangle
            indices.Push( uint16(i) );
            indices.Push( uint16(i+(numTiles+1)) );
            indices.Push( uint16(i+1) );

            // Second triangle
            indices.Push( uint16(i+1) );
            indices.Push( uint16(i+(numTiles+1)) ) ;
            indices.Push( uint16(i+(numTiles+2)) );
        }
    }

    const GeometryBufferPtr& gb = rend->getGeometryBuffer();
    gb->setIndex((uint8*) &indices.Front(), indices.Size() * sizeof(uint16));

}

//-----------------------------------//

void Cell::rebuildNormals()
{
    rebuildFaceNormals();
    rebuildAveragedNormals();
}

//-----------------------------------//

static Vector3 CalculateTriangleNormal( const Vector3& v1, const Vector3& v2, const Vector3& v3 )
{
    Vector3 vec1 = v2 - v1;
    Vector3 vec2 = v3 - v1;
    Vector3 normal = vec1.cross(vec2);

    return normal.normalize();
}

#define index(i) (indexData[i])

void Cell::rebuildFaceNormals()
{
    if( heights.Empty() ) return;

    const GeometryBufferPtr& gb = rend->getGeometryBuffer();

    faceNormals.Clear();

    LogInfo( "Rebuilding face normals of cell (%hd, %hd)", x, y );

    uint32 numIndices = gb->getNumIndices();

    Vector3* vertexData = (Vector3*) gb->getAttribute(VertexAttribute::Position, 0);
    int8 vertexStride = gb->getAttributeStride(VertexAttribute::Position);
    
    uint16* indexData = (uint16*) gb->indexData.Buffer();

    for( size_t i = 0; i < numIndices; i += 3 )
    {
        uint8 offset = i * vertexStride;

        const Vector3& v1 = vertexData[index(i+0) + offset];
        const Vector3& v2 = vertexData[index(i+1) + offset];
        const Vector3& v3 = vertexData[index(i+2) + offset];

        Vector3 normal = CalculateTriangleNormal(v1, v2, v3);
        faceNormals.Push( normal );
    }

    const uint numTiles = settings->NumberTiles;
    assert( faceNormals.Size() == numTiles*numTiles*2 );
}

//-----------------------------------//

// Resources:
// http://www.gamedev.net/community/forums/topic.asp?topic_id=163625
// http://www.gamedev.net/reference/articles/article2264.asp
//

#define isRegular(x,y) ((x>=1u) && (x<=(numTiles-1u)) \
                        && (y>=1u) && (y<=(numTiles-1u)))

byte Cell::getNeighborFaces( uint i, Vector<uint>& ns )
{
    const int numTiles = settings->NumberTiles;
    uint facesPerRow = numTiles*2;
    
    uint row = i / (numTiles+1);
    uint col = i % (numTiles+1);
    
    byte n = 0;

    if( isRegular(col, row) )
    {
        uint startFace = (row-1)*facesPerRow+(col*2);

        ns[n++] = startFace-1;
        ns[n++] = startFace;
        ns[n++] = startFace+1;

        ns[n++] = startFace+facesPerRow-2;
        ns[n++] = startFace+facesPerRow-1;
        ns[n++] = startFace+facesPerRow;
    }
    else
    {
        ns[n++] = 0;
    }

    return n;
}

//-----------------------------------//

void Cell::rebuildAveragedNormals()
{
    if( faceNormals.Empty() ) return;

    const GeometryBufferPtr& gb = rend->getGeometryBuffer();

    Vector3* vs = (Vector3*) gb->getAttribute(VertexAttribute::Position, 0);
    assert( vs != nullptr );

    // Averaged per-vertex normals.
    Vector<Vector3> normals;

    LogInfo( "Rebuilding average per-vertex normals of cell (%hd, %hd)", x, y );

    Vector<uint> ns;
    ns.Resize(6);

    uint32 numVertices = gb->getNumVertices();

    for(size_t i = 0; i < numVertices; i++)
    {
        uint8 n = getNeighborFaces(i, ns);

        Vector3 average;
        
        for( uint8 j = 0; j < n; j++ )
            average += faceNormals[ ns[j] ];

        average /= n;
        average.normalize();

        normals.Push( average );
    }

    assert( normals.Size() ==  gb->getNumVertices() );
    gb->set( VertexAttribute::Normal, normals );

    gb->forceRebuild();
}

//-----------------------------------//

NAMESPACE_ENGINE_END