
namespace Flood
{
    public static class RectExtensions
    {
        public static int GetLeft(this Rectangle rect)
        {
            return rect.X;
        }

        public static int GetRight(this Rectangle rect)
        {
            return rect.X + rect.Width;
        }

        public static int GetTop(this Rectangle rect)
        {
            return rect.Y;
        }

        public static int GetBottom(this Rectangle rect)
        {
            return rect.Y + rect.Height;
        }

        public static float GetLeft(this RectangleF rect)
        {
            return rect.X;
        }

        public static float GetRight(this RectangleF rect)
        {
            return rect.X + rect.Width;
        }

        public static float GetTop(this RectangleF rect)
        {
            return rect.Y;
        }

        public static float GetBottom(this RectangleF rect)
        {
            return rect.Y + rect.Height;
        }
    }

}
