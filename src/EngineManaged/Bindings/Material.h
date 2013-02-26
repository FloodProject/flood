/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

#include <Graphics/Resources/Material.h>
#include "Reflection.h"
#include "Memory.h"
#include "Vector.h"
#include "Resource.h"

namespace Flood
{
    enum struct BlendSource;
    enum struct BlendDestination;
    enum struct DepthCompare;
    ref class Class;
    ref class Allocator;
    ref class Field;
    ref class Type;
    enum struct TypeKind : unsigned char;
    ref class ReflectionContext;
    enum struct ReflectionWalkType : unsigned char;
    enum struct FieldQualifier : unsigned short;
    ref class Handle;
    ref class Material;
    enum struct ResourceGroup;
    value struct Vector2;
    value struct Vector3;
    ref class TextureUnit;
    enum struct TextureFilterMode;
    enum struct TextureMipMode;
    enum struct TextureWrapMode;

    public enum struct BlendSource
    {
        Zero = 0,
        One = 1,
        DestinationColor = 2,
        InverseDestinationColor = 3,
        SourceAlpha = 4,
        InverseSourceAlpha = 5,
        DestinationAlpha = 6,
        InverseDestinationAlpha = 7,
        SourceAlphaSaturate = 8
    };

    public enum struct BlendDestination
    {
        Zero = 0,
        One = 1,
        SourceColor = 2,
        InverseSourceColor = 3,
        SourceAlpha = 4,
        InverseSourceAlpha = 5,
        DestinationAlpha = 6,
        InverseDestinationAlpha = 7
    };

    public enum struct DepthCompare
    {
        Never = 0,
        Less = 1,
        Equal = 2,
        LessOrEqual = 3,
        Greater = 4,
        NotEqual = 5,
        GreaterOrEqual = 6,
        Always = 7
    };

    public ref class Material : Resource
    {
    public:
        property ::Material* NativePtr;

        Material(::Material* native);
        Material(System::IntPtr native);
        Material();
        Material(System::String^ name);
        property System::String^ Name;
        property bool CullBackfaces;
        property Flood::DepthCompare DepthCompare;
        property bool DepthTest;
        property bool DepthWrite;
        property Flood::Vector2 DepthRange;
        property Flood::Vector2 DepthOffset;
        property bool AlphaTest;
        property bool LineSmooth;
        property float LineWidth;
        property Flood::BlendSource Source;
        property Flood::BlendDestination Destination;
        property bool _isBlendingEnabled;
        Flood::Class^ GetType();
        Flood::Class^ GetStaticType();
        Flood::ResourceGroup GetResourceGroup();
        System::String^ GetName();
        void SetName(System::String^ v);
        void SetShader(System::String^ name);
        bool GetDepthWrite();
        void SetDepthWrite(bool v);
        bool GetDepthTest();
        void SetDepthTest(bool v);
        Flood::DepthCompare GetDepthCompare();
        void SetDepthCompare(Flood::DepthCompare v);
        Flood::Vector2 GetDepthRange();
        void SetDepthRange(Flood::Vector2 v);
        Flood::Vector2 GetDepthOffset();
        void SetDepthOffset(Flood::Vector2 v);
        bool GetAlphaTest();
        void SetAlphaTest(bool v);
        float GetLineWidth();
        void SetLineWidth(float v);
        bool GetLineSmoothing();
        void SetLineSmoothing(bool v);
        bool GetBackfaceCulling();
        void SetBackfaceCulling(bool v);
        Flood::BlendSource GetBlendSource();
        Flood::BlendDestination GetBlendDestination();
        bool IsBlendingEnabled();
        void SetBlending(Flood::BlendSource _214, Flood::BlendDestination _215);
        void SetTexture(unsigned char unit, System::String^ name);
        Flood::TextureUnit^ GetTextureUnit(unsigned char unit);
        void Init();
    };
}
