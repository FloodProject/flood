using System;

namespace Flood.GUIv2
{
    /// <summary>
    /// Represents outer spacing.
    /// </summary>
    public struct Margin : IEquatable<Margin>
    {
        public int Top;
        public int Bottom;
        public int Left;
        public int Right;

        // common values
        public static Margin Zero = new Margin(0);
        public static Margin One = new Margin(1);
        public static Margin Two = new Margin(2);
        public static Margin Three = new Margin(3);
        public static Margin Four = new Margin(4);
        public static Margin Five = new Margin(5);
        public static Margin Six = new Margin(6);
        public static Margin Seven = new Margin(7);
        public static Margin Eight = new Margin(8);
        public static Margin Nine = new Margin(9);
        public static Margin Ten = new Margin(10);

        public Margin(int left, int top, int right, int bottom)
        {
            Top = top;
            Bottom = bottom;
            Left = left;
            Right = right;
        }

        public Margin(int uniform)
        {
            Top = uniform;
            Bottom = uniform;
            Left = uniform;
            Right = uniform;
        }
        
        public Margin(int horizontal, int vertical)
        {
            Top = vertical;
            Bottom = vertical;
            Left = horizontal;
            Right = horizontal;
        }

        public bool Equals(Margin other)
        {
            return other.Top == Top && other.Bottom == Bottom && other.Left == Left && other.Right == Right;
        }

        public static bool operator==(Margin lhs, Margin rhs)
        {
            return lhs.Equals(rhs);
        }

        public static bool operator!=(Margin lhs, Margin rhs)
        {
            return !lhs.Equals(rhs);
        }

        public override bool Equals(object obj)
        {
            if (ReferenceEquals(null, obj)) return false;
            if (obj.GetType() != typeof (Margin)) return false;
            return Equals((Margin) obj);
        }

        public override int GetHashCode()
        {
            unchecked
            {
                int result = Top;
                result = (result*397) ^ Bottom;
                result = (result*397) ^ Left;
                result = (result*397) ^ Right;
                return result;
            }
        }
    }
}
