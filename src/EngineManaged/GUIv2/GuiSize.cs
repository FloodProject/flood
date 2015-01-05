

using System;

namespace Flood.GUIv2
{
    public struct GUISize
    {
        public const uint MaxSize = uint.MaxValue;
 
        public readonly uint X;
        public readonly uint Y;

        public readonly bool isXSet;
        public readonly bool isYSet;

        public GUISize(uint? x, uint? y)
        {
            X = (x != null) ? x.Value : 0;
            Y = (y != null) ? y.Value : 0;

            isXSet = (x != null);
            isYSet = (y != null);
        }

        public static GUISize operator+ (GUISize size1, GUISize size2)
        {

            uint? x = (size1.isXSet || size2.isXSet) ? AddOverflow(size1.X, size2.X) : null;
            uint? y = (size1.isYSet || size2.isYSet) ? AddOverflow(size1.Y, size2.Y) : null;

            return new GUISize(x, y);
        }

        public static bool operator== (GUISize size1, GUISize size2)
        {
            return size1.X == size2.X && size1.Y == size2.Y && (size1.isXSet == size2.isXSet) &&
                   (size1.isYSet == size2.isYSet);
        } 

        public static bool operator!= (GUISize size1, GUISize size2)
        {
            return !(size1 == size2);
        } 

        public static bool operator<= (GUISize size1, GUISize size2)
        {
            return size1 == size2 || size1 < size2;
        }

        public static bool operator < (GUISize size1, GUISize size2)
        {
            return size1.X < size2.X && size1.Y < size2.Y;
        }

        public static bool operator>= (GUISize size1, GUISize size2)
        {
            return size1 == size2 || size1 > size2;
        }

        public static bool operator > (GUISize size1, GUISize size2)
        {
            return size1.X > size2.X && size1.Y > size2.Y;
        }

        public GUISize Add(GUISize size, bool addX)
        {
            return addX ? AddX(size) : AddY(size);
        }

        private static uint? AddOverflow(uint first, uint second)
        {
            return (first == MaxSize || second == MaxSize) ? MaxSize : first + second;
        }

        public GUISize AddX(GUISize size)
        {
            var x = (isXSet || size.isXSet)? AddOverflow(X, size.X) : null;
            return new GUISize(x, (isYSet)? (uint?)Y : null);
        }

        public GUISize AddY(GUISize size)
        {
            var y = (isYSet || size.isYSet) ? AddOverflow(Y, size.Y) : null;
            return new GUISize((isXSet) ? (uint?)X : null, y);
        }

        public GUISize Max(GUISize size, bool maxX)
        {
            return maxX ? MaxX(size) : MaxY(size);
        }

        public GUISize MaxX(GUISize size)
        {
            var x = (isXSet || size.isXSet) ? (uint?)Math.Max(X, size.X) : null;
            return new GUISize(x, (isYSet) ? (uint?)Y : null);
        }

        public GUISize MaxY(GUISize size)
        {
            var y = (isYSet || size.isYSet) ? (uint?)Math.Max(Y, size.Y) : null;
            return new GUISize((isXSet) ? (uint?)X : null, y);
        }

        //public static implicit operator GUISize(Vector2i vector)
        //{
        //    uint? x = (vector.X < 0) ? null : (vector.X == Control.MaxNotSet)?  MaxSize : (uint?)vector.X;
        //    uint? y = (vector.Y < 0) ? null : (vector.Y == Control.MaxNotSet) ? MaxSize : (uint?)vector.Y; 

        //    return new GUISize(x, y);
        //}

        //public static implicit operator Vector2i(GUISize size)
        //{
        //    var x = (!size.isXSet) ? Control.MinNotSet : (size.X == MaxSize) ? Control.MaxNotSet : (int) size.X; 
        //    var y = (!size.isYSet) ? Control.MinNotSet : (size.Y == MaxSize) ? Control.MaxNotSet : (int) size.Y;  

        //    return new Vector2i(x, y);
        //}
    }
}
