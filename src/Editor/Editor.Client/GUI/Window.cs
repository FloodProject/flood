using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Flood.GUI.Controls;
using Flood.GUI.Input;
using Flood.GUI.Renderers;
using Flood.GUI.Skins;

namespace Editor.Client.Gui
{
    public abstract class Window
    {
        /// <summary>
        /// Renderer of the GUI.
        /// </summary>
        public Renderer Renderer { get; private set; }

        /// <summary>
        /// Skin of the GUI.
        /// </summary>
        public Skin Skin  { get; private set; }

        public Canvas Canvas  { get; private set; }

        public void Init(Renderer renderer, string textureName)
        {
            Renderer = renderer;
            Skin = new TexturedSkin(renderer, textureName);
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
