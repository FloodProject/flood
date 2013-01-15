using Flood.Editor.GUI.Controls;
using System;
using Gwen.Control;

namespace Flood.Editor
{
    public class EditorWindow : IDisposable
    {
        /// <summary>
        /// Toolbar of the main editor window.
        /// </summary>
        public ToolBar ToolBar { get; private set; }

        /// <summary>
        /// Document tabs of the main editor window.
        /// </summary>
        public DocumentTab DocumentTab { get; private set; }

        /// <summary>
        /// Canvas of the main editor window.
        /// </summary>
        public Canvas Canvas { get; private set; }

        public Gwen.Renderer.Base renderer;
        public Gwen.Skin.Base skin;

        public void Dispose()
        {
            Canvas.Dispose();
            renderer.Dispose();
            skin.Dispose();
        }

        internal event Action GUIInitiated;

        public void Init(Gwen.Renderer.Base renderer, string textureName)
        {
            this.renderer = renderer;
            skin = new Gwen.Skin.TexturedBase(renderer, textureName);
            Canvas = new Gwen.Control.Canvas(skin);

            ToolBar = new ToolBar(Canvas);
            DocumentTab = new DocumentTab(Canvas);

            if (GUIInitiated != null)
                GUIInitiated.Invoke();
        }

        public void Render()
        {
            Canvas.RenderCanvas();
        }
    }
}
