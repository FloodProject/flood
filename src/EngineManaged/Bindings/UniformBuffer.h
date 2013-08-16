/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

#include "CppSharp.h"
#include <Graphics/UniformBuffer.h>

namespace Flood
{
    enum struct UniformDataType : unsigned char;
    ref class UniformBuffer;
    value struct Color;
    value struct Matrix4x3;
    value struct Matrix4x4;
    value struct UniformBufferElement;
    value struct Vector3;

    public enum struct UniformDataType : unsigned char
    {
        Scalar_F = 0,
        Scalar_I = 1,
        Vector2_F = 2,
        Vector3_F = 3,
        Matrix2_F = 4,
        Matrix3_F = 5,
        Matrix4_F = 6,
        Matrix2x3_F = 7,
        Matrix3x2_F = 8,
        Matrix2x4_F = 9,
        Matrix4x2_F = 10,
        Matrix3x4_F = 11,
        Matrix4x3_F = 12
    };

    /// <summary>
    /// Uniforms are named constants that can be set in programs. These are
    /// allocated from a special frame bump allocator, that frees all the memory
    /// when the frame ends.
    /// </summary>
    public value struct UniformBufferElement
    {
    public:

        UniformBufferElement(::UniformBufferElement* native);
        UniformBufferElement(System::IntPtr native);
        System::String^ Name;
        Flood::UniformDataType Type;
        unsigned short Count;
        cli::array<unsigned char>^ Data;
    };

    /// <summary>
    /// Represents a uniform buffer.
    /// </summary>
    public ref class UniformBuffer : ICppInstance
    {
    public:

        property ::UniformBuffer* NativePtr;
        property System::IntPtr Instance
        {
            virtual System::IntPtr get();
            virtual void set(System::IntPtr instance);
        }

        UniformBuffer(::UniformBuffer* native);
        UniformBuffer(System::IntPtr native);
        UniformBuffer();

        Flood::UniformBufferElement GetElement(System::String^ name, unsigned int size);

        void RemoveUniform(System::String^ slot);

        void SetUniform(System::String^ slot, int data);

        void SetUniform(System::String^ slot, float value);

        void SetUniform(System::String^ slot, System::Collections::Generic::List<Flood::Vector3>^ vec);

        void SetUniform(System::String^ slot, System::Collections::Generic::List<Flood::Color>^ vec);

        void SetUniform(System::String^ slot, Flood::Vector3 vec);

        void SetUniform(System::String^ slot, Flood::Matrix4x3 _0);

        void SetUniform(System::String^ slot, Flood::Matrix4x4 _0);

        void SetUniform(System::String^ slot, System::Collections::Generic::List<Flood::Matrix4x4>^ vec);

        virtual bool Equals(System::Object^ object) override;

        virtual int GetHashCode() override;

    };
}
