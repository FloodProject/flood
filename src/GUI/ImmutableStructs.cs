using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Text;

namespace Gwen
{
    public struct PointImmutable
    {
        public readonly int X;
        public readonly int Y;
        public PointImmutable(int x, int y)
        {
            X = x;
            Y = y;
        }

        public static implicit operator Point(PointImmutable p){
            return new Point(p.X,p.Y);
        }

        public static implicit operator PointImmutable(Point p)
        {
            return new PointImmutable(p.X, p.Y);
        }
    }

    public struct RectangleImmutable
    {
        public readonly int X;
        public readonly int Y;
        public readonly int Width;
        public readonly int Height;

        public RectangleImmutable(int x, int y, int width, int height)
        {
            X = x;
            Y = y;
            Width = width;
            Height = height;
        }

        public int Bottom { get{ return Y + Height;}}
        public int Top { get { return Y; } }
        public int Right { get { return X + Width; } }
        public int Left { get { return X; } }

        public static implicit operator Rectangle(RectangleImmutable r)
        {
            return new Rectangle(r.X, r.Y, r.Width, r.Height);
        }

        public static implicit operator RectangleImmutable(Rectangle r)
        {
            return new RectangleImmutable(r.X, r.Y, r.Width, r.Height);
        }
    }
}
