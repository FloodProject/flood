/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

#include "Core/References.h"
#include "Core/Math/Vector.h"
#include "Graphics/Buffer.h"
#include "Graphics/VertexBuffer.h"

NAMESPACE_GRAPHICS_BEGIN

//-----------------------------------//

/**
 * Represents a buffer with geometry data. You have to associate the vertex
 * data layout to the buffer so it can be used by the engine.
 */

class API_GRAPHICS GeometryBuffer : public ReferenceCounted
{
public:

    GeometryBuffer();
    GeometryBuffer(BufferUsage, BufferAccess);
    
    ~GeometryBuffer();

    // Gets/sets the buffer usage type.
    ACCESSOR(BufferUsage, BufferUsage, usage)

    // Gets/sets the buffer access type.
    ACCESSOR(BufferAccess, BufferAccess, access)

    // Forces rebuild of geometry data.
    void forceRebuild();

    // Clears the buffer.
    void clear();

    // Sets the attribute data in the buffer.
    void set(VertexAttribute, uint8* data, uint32 size);

    // Sets the data in the buffer.
    void set(uint8* data, uint32 size);

    // Adds data to the buffer.
    void add(uint8* data, uint32 size);

    // Sets the index data in the index buffer.
    void setIndex(uint8* data, uint32 size);

    // Adds index data to the index buffer.
    void addIndex(uint8* data, uint32 size);

    // Adds an index to the index buffer.
    void addIndex(uint16 index);

    // Returns if the buffer has indexed data.
    bool isIndexed() const;

    // Gets a pointer to the attribute data.
    float* getAttribute(VertexAttribute, uint32 i) const;

    // Gets the stride of the vertex attribute.
    int8 getAttributeStride(VertexAttribute) const;

    // Gets the number of vertices in the buffer.
    uint32 getNumVertices() const;

    // Gets the number of indices in the buffer.
    uint32 getNumIndices() const;

    // Clears index data buffer.
    void clearIndexes();

    template<typename T>
    FLD_IGNORE void set(VertexAttribute attr, const Vector<T>& data)
    {
        if( data.Empty() ) return;

        uint32 sizeInBytes = data.Size() * sizeof(T);

        if( !declarations.find(attr) )
        {
            VertexElement decl(attr, VertexDataType::Float, sizeof(T) / sizeof(float));
            decl.stride = 0;
            decl.offset = this->data.Size();
            decl.size = sizeInBytes;

            declarations.add(decl);

            add((uint8*)data.Buffer(), sizeInBytes);
        }
        else
        {
            set(attr, (uint8*)data.Buffer(), sizeInBytes);
        }
    }
    
    // Buffer usage.
    BufferUsage usage;
    
    // Buffer access.
    BufferAccess access;

    // Holds the vertex data.
    Vector<uint8> data;

    // Holds the index data.
    Vector<uint8> indexData;

    // Holds the index size in bits.
    uint8 indexSize;

    // Holds if the buffer needs to be rebuilt.
    bool needsRebuild;

    // Hash of the contents of the buffer.
    uint32 hash;

    // Holds the vertex declarations.
    VertexDeclaration declarations;
};

TYPEDEF_INTRUSIVE_POINTER_FROM_TYPE( GeometryBuffer );

//-----------------------------------//

NAMESPACE_GRAPHICS_END