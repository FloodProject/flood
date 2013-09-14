using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using FLDEditor.Services;
using Flood;
using Flood.Editor.Client;
using Flood.Editor.Client.GUI;
using Flood.GUI.Controls;
using Flood.GUI.Renderers;
using Flood.GUI.Skins;
using Flood.RPC;

namespace FLDEditor
{
    public class EditorWindow : IDisposable
    {
        /// <summary>
        /// Renderer of the GUI.
        /// </summary>
        public GwenRenderer GuiRenderer { get; private set; }

        public GwenInput Input { get; private set; }

        public Canvas Canvas { get; private set; }

        public EditorWindow()
        {
            InitializeGui();
        }

        ~EditorWindow()
        {
            InitializeGui();
        }

        public void Dispose()
        {
            GuiRenderer.Dispose();
        }

        void InitializeGui()
        {
            GuiRenderer = new GwenRenderer();

            var textureName = "DefaultSkin.png";
            var defaultFont = new Flood.GUI.Font("Vera.ttf", 16);

            var resMan = FloodEngine.GetEngine().ResourceManager;
            var options = new ResourceLoadOptions {Name = textureName, AsynchronousLoad = false};
            var imageHandle = resMan.LoadResource<Image>(options);
            if (imageHandle.Id == 0)
                return;

            var skin = new TexturedSkin(GuiRenderer, imageHandle, defaultFont);
            Canvas = new Canvas(skin);

            var container = new Container(Canvas);
            var paneGroup = new PaneGroup(container);
            container.InsertPanel(paneGroup);
            paneGroup.AddPane(new Pane { Title = "PANE1" });
            paneGroup.AddPane(new Pane { Title = "PANE2" });

            Input = new GwenInput(FloodEngine.GetEngine().InputManager);
            Input.Initialize(Canvas);
        }

        public void Render(RenderBlock rb)
        {
            GuiRenderer.Clear();
            Canvas.RenderCanvas();
            GuiRenderer.Render(rb);
        }

        public void Update()
        {
        }

        public void SetSize(int x, int y)
        {
            Canvas.SetSize(x, y);
        }
    }
}
