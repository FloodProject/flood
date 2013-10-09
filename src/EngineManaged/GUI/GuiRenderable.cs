
using System;
using Flood;
using Flood.Windows;
using GUI.Controls;
using GUI.Skins;

namespace GUI
{
    public class GuiRenderable : WindowRenderable, IDisposable
    {
        public GwenRenderer GuiRenderer { get; private set; }

        public GwenInput Input { get; private set; }

        public Canvas Canvas { get; private set; }

        public GuiRenderable()
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
            var defaultFont = new Font("Vera.ttf", 16);

            var resMan = FloodEngine.GetEngine().ResourceManager;
            var options = new ResourceLoadOptions {Name = textureName, AsynchronousLoad = false};
            var imageHandle = resMan.LoadResource<Image>(options);
            if (imageHandle.Id == 0)
                return;

            var skin = new TexturedSkin(GuiRenderer, imageHandle, defaultFont);
            Canvas = new Canvas(skin);

            Input = new GwenInput(FloodEngine.GetEngine().InputManager);
            Input.Initialize(Canvas);
        }

        public override void Render(RenderBlock renderBlock)
        {
            GuiRenderer.Clear();
            Canvas.RenderCanvas();
            GuiRenderer.Render(renderBlock);
        }

        public override void Resize(int width, int height)
        {
            Canvas.SetSize(width, height);
        }
    }
}
