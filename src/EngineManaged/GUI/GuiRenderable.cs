
using Flood.GUI.Controls;
using Flood.GUI.Skins;
using Flood.Windows;
using System;

namespace Flood.GUI
{
    public class GuiRenderable : WindowRenderable, IDisposable
    {
        public GwenRenderer GuiRenderer { get; private set; }

        public GwenInput Input { get; private set; }

        public Canvas Canvas { get; private set; }

        public Skin Skin { get; private set; }

        public GuiRenderable()
        {
            InitializeGui();
        }

        public void Dispose()
        {
            GuiRenderer.Dispose();
            Skin.Dispose();
            Canvas.Dispose();
        }

        void InitializeGui()
        {
            GuiRenderer = new GwenRenderer();

            var textureName = "DefaultSkin.png";
            var defaultFont = new Font("Vera.ttf", 16);

            var resMan = Engine.GetEngine().ResourceManager;
            var options = new ResourceLoadOptions {Name = textureName, AsynchronousLoad = false};
            var imageHandle = resMan.LoadResource<Image>(options);
            if (imageHandle.Id == 0)
                return;

            Skin = new TexturedSkin(GuiRenderer, imageHandle, defaultFont);
            Canvas = new Canvas(Skin);

            Input = new GwenInput(Engine.GetEngine().InputManager);
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
