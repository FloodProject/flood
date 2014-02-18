/************************************************************************
*
* Flood Project © (2008-201x)
* Licensed under the simplified BSD license. All rights reserved.
*
************************************************************************/

#pragma once

#include "CppSharp.h"
#include <Core/Math/Rectangle.h>

namespace Flood
{
    value struct Rectangle;
    value struct RectangleF;
}

namespace Flood
{
    public value struct Rectangle
    {
    public:

        Rectangle(::Rectangle* native);
        Rectangle(System::IntPtr native);
        Rectangle(int x, int y, int width, int height);

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

        /// <summary>
        /// Returns true if this rectangle is contained in rect.
        /// </summary>
        bool IsContainedIn(Flood::Rectangle rect);

        private:
        int __X;
        int __Y;
        int __Width;
        int __Height;
    };

    public value struct RectangleF
    {
    public:

        RectangleF(::RectangleF* native);
        RectangleF(System::IntPtr native);
        RectangleF(float x, float y, float width, float height);

        property float X
        {
            float get();
            void set(float);
        }

        property float Y
        {
            float get();
            void set(float);
        }

        property float Width
        {
            float get();
            void set(float);
        }

        property float Height
        {
            float get();
            void set(float);
        }

        /// <summary>
        /// Returns true if this rectangle is contained in rect.
        /// </summary>
        bool IsContainedIn(Flood::RectangleF rect);

        private:
        float __X;
        float __Y;
        float __Width;
        float __Height;
    };
}
