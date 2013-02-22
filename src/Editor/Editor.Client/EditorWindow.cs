//using Flood.Editor.Controls;
using System;
using System.ComponentModel.Composition;
using EngineManaged.GUI;
using EngineManaged.GUI.Controls;
using Flood.Editor.Controls;

namespace Flood.Editor
{
    [Export]
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

        /// <summary>
        /// Size of the main editor window.
        /// </summary>
        public Vector2 Size
        {
            get { return size; }
            set
            {
                size = value;
                //OnResize();
            }
        }

        private Vector2 size;

        public void SetCanvasSize(int x, int y)
        {
            Canvas.SetSize(x, y);
        }

        /// <summary>
        /// Renderer of the GUI.
        /// </summary>
        private EngineManaged.GUI.Renderer.Base renderer;

        /// <summary>
        /// Skin of the GUI.
        /// </summary>
        private EngineManaged.GUI.Skin.Base skin;

        public void Dispose()
        {
            Canvas.Dispose();
            renderer.Dispose();
            skin.Dispose();
        }

        internal event Action GUIInitiated;

        public void Init(EngineManaged.GUI.Renderer.Base renderer, string textureName)
        {
            this.renderer = renderer;
            skin = new EngineManaged.GUI.Skin.TexturedBase(renderer, textureName);
            Canvas = new EngineManaged.GUI.Controls.Canvas(skin);

            ToolBar = new ToolBar(Canvas);
            DocumentTab = new DocumentTab(Canvas);

            /*var label = new Label();
            label.Text = "YOLO!";
            label.SetSize(200, 200);
            label.SetPosition(200,200);
            Canvas.AddChild(label);
            */

            if (GUIInitiated != null)
                GUIInitiated.Invoke();
        }

        public void Render()
        {
            Canvas.RenderCanvas();
        }

        public void SetSize()
        {
            Canvas.RenderCanvas();
        }
    }
}
