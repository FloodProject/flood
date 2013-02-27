using System;
using System.Drawing;
using System.IO;

namespace Flood.GUI
{
    /// <summary>
    /// Represents a texture.
    /// </summary>
    public class Texture : IDisposable
    {
        /// <summary>
        /// Texture name. Usually file name, but exact meaning depends on renderer.
        /// </summary>
        public String Name { get; set; }

        /// <summary>
        /// Renderer data.
        /// </summary>
        public object RendererData { get; set; }

        /// <summary>
        /// Indicates that the texture failed to load.
        /// </summary>
        public bool Failed { get; set; }

        /// <summary>
        /// Texture width.
        /// </summary>
        public int Width { get; set; }

        /// <summary>
        /// Texture height.
        /// </summary>
        public int Height { get; set; }

        private readonly Renderers.Renderer m_Renderer;

        /// <summary>
        /// Initializes a new instance of the <see cref="Texture"/> class.
        /// </summary>
        /// <param name="renderer">Renderer to use.</param>
        public Texture(Renderers.Renderer renderer)
        {
            m_Renderer = renderer;
            Width = 4;
            Height = 4;
            Failed = false;
        }

        /// <summary>
        /// Loads the specified texture.
        /// </summary>
        /// <param name="name">Texture name.</param>
        public void Load(String name)
        {
            Name = name;
            m_Renderer.LoadTexture(this);
        }

        /// <summary>
        /// Initializes the texture from a bitmap.
        /// </summary>
        /// <param name="image">Image bitmap.</param>
        public void LoadBitmap(Bitmap image)
        {
            Width = image.Width;
            Height = image.Height;
            m_Renderer.LoadTextureBitmap(this, image);
        }

        /// <summary>
        /// Performs application-defined tasks associated with freeing, releasing, or resetting unmanaged resources.
        /// </summary>
        public void Dispose()
        {
            m_Renderer.FreeTexture(this);
            GC.SuppressFinalize(this);
        }

#if DEBUG
        ~Texture()
        {
            throw new InvalidOperationException(String.Format("IDisposable object finalized: {0}", GetType()));
            //Debug.Print(String.Format("IDisposable object finalized: {0}", GetType()));
        }
#endif

   }
}
