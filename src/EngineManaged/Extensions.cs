
namespace Flood
{
    public static class RectExtensions
    {
        public static int GetLeft(this Rect rect)
        {
            return rect.X;
        }

        public static int GetRight(this Rect rect)
        {
            return rect.X + rect.Width;
        }

        public static int GetTop(this Rect rect)
        {
            return rect.Y;
        }

        public static int GetBottom(this Rect rect)
        {
            return rect.Y + rect.Height;
        }
    }

    public static class TextureExtensions
    {
        public static void LoadImage(this Texture tex, string imageName)
        {
            var resMan = FloodEngine.GetEngine().GetResourceManager();
            var options = new ResourceLoadOptions {Name = imageName, AsynchronousLoad = false};
            tex.SetImage(resMan.LoadResource<Image>(options));
        }
    }
}
