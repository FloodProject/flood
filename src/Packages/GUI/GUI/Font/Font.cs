using System;
using Flood;

namespace GUI
{
    /// <summary>
    /// Represents font resource.
    /// </summary>
    public class Font : IDisposable
    {
        /// <summary>
        /// Font face name. Exact meaning depends on renderer.
        /// </summary>
        public String FaceName { get; private set; }

        /// <summary>
        /// Font size.
        /// </summary>
        public int Size { get; set; }

        /// <summary>
        /// Enables or disables font smoothing (default: disabled).
        /// </summary>
        public bool Smooth { get; set; }

        //public bool Bold { get; set; }
        //public bool DropShadow { get; set; }
        
        /// <summary>
        /// This should be set by the renderer if it tries to use a font where it's null.
        /// </summary>
        public ResourceHandle<Flood.Font> EngineFont { get; set; }

        /// <summary>
        /// This is the real font size, after it's been scaled by Renderer.Scale()
        /// </summary>
        public float RealSize { get; set; }

        /// <summary>
        /// Initializes a new instance of the <see cref="Font"/> class.
        /// </summary>
        /// <param name="faceName">Face name.</param>
        /// <param name="size">Font size.</param>
        public Font(String faceName, int size = 10)
        {
            var ttf = (faceName.EndsWith(".ttf")) ? faceName : faceName + ".ttf";
            EngineFont = FloodResourceManager.GetResourceManager().LoadResource<Flood.Font>(ttf);
            FaceName = faceName;
            Size = size;
            Smooth = false;
        }

        /// <summary>
        /// Performs application-defined tasks associated with freeing, releasing, or resetting unmanaged resources.
        /// </summary>
        public void Dispose()
        {
            GC.SuppressFinalize(this);
        }
        
#if DEBUG
        ~Font()
        {
            throw new InvalidOperationException(String.Format("IDisposable object finalized: {0}", GetType()));
            //Debug.Print(String.Format("IDisposable object finalized: {0}", GetType()));
        }
#endif

    }
}
