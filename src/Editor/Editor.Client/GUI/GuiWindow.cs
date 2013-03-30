using System;
using Flood.GUI.Controls;
using Flood.GUI.Renderers;
using Flood.GUI.Skins;

namespace Flood.Editor.Client.Gui
{
    public abstract class GuiWindow : IDisposable
    {
        /// <summary>
        /// Native GUI window.
        /// </summary>
        public Window NativeWindow  { get; set; }

        /// <summary>
        /// Renderer of the GUI.
        /// </summary>
        public Renderer Renderer { get; private set; }

        /// <summary>
        /// Skin of the GUI.
        /// </summary>
        public Skin Skin { get; private set; }

        public Canvas Canvas { get; private set; }

        public void Dispose()
        {
            Dispose(true);
            GC.SuppressFinalize(this);
        }

        protected virtual void Dispose(bool disposing)
        {
            if (!disposing) return;

            Canvas.Dispose();
            Skin.Dispose();
            Renderer.Dispose();
        }

        public void Init(Renderer renderer, string textureName, Flood.GUI.Font defaultFont)
        {
            Renderer = renderer;
            Skin = new TexturedSkin(renderer, textureName, defaultFont);
            Canvas = new Canvas(Skin);

            Init();
        }

        protected abstract void Init();

        public void Render()
        {
            Canvas.RenderCanvas();
        }
    }
}
