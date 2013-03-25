/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

#include <Engine/RectangleBinPack/Rect.h>
#include "ResourceHandle.h"

namespace Flood
{
    ref class DisjointRectCollection;
    ref class Rect;
    ref class RectSize;

    public ref class RectSize
    {
    public:
        property ::RectSize* NativePtr;

        RectSize(::RectSize* native);
        RectSize(System::IntPtr native);
        property int Width
        {
            int get();
            void set(int);
        }
        property int Height
        {
            int get();
            void set(int);
        }
    };

    public ref class Rect
    {
    public:
        property ::Rect* NativePtr;

        Rect(::Rect* native);
        Rect(System::IntPtr native);
        property int X
        {
            int get();
            void set(int);
        }
        property int Y
        {
            int get();
            void set(int);
        }
        property int Width
        {
            int get();
            void set(int);
        }
        property int Height
        {
            int get();
            void set(int);
        }
    };

    public ref class DisjointRectCollection
    {
    public:
        property ::DisjointRectCollection* NativePtr;

        DisjointRectCollection(::DisjointRectCollection* native);
        DisjointRectCollection(System::IntPtr native);
        property System::Collections::Generic::List<Flood::Rect^>^ Rects
        {
            System::Collections::Generic::List<Flood::Rect^>^ get();
            void set(System::Collections::Generic::List<Flood::Rect^>^);
        }
        bool Add(Flood::Rect^ r);
        void Clear();
        bool Disjoint(Flood::Rect^ r);
        bool Disjoint(Flood::Rect^ a, Flood::Rect^ b);
    };

    public ref class FloodRect
    {
    public:
        /// <summary>
        /// Performs a lexicographic compare on (rect short side, rect long side).
        /// </summary>
        static int CompareRectShortSide(Flood::Rect^ a, Flood::Rect^ b);
        /// <summary>
        /// Performs a lexicographic compare on (x, y, width, height).
        /// </summary>
        static int NodeSortCmp(Flood::Rect^ a, Flood::Rect^ b);
        /// <summary>
        /// Returns true if a is contained in b.
        /// </summary>
        static bool IsContainedIn(Flood::Rect^ a, Flood::Rect^ b);
    };
}
