
using System.Collections.Generic;
using Flood.Remoting.Metadata;

namespace Flood.Windows
{
    [DataObject(6)]
    public class WindowRenderable
    {
        public virtual void Init()
        {
        }

        public virtual void Render(RenderBlock renderBlock)
        {
        }

        public virtual void Update()
        {
        }

        public virtual void Resize(int width, int height)
        {
        }
    }

    [DataObject(5)]
    public class Window
    {
        private RenderDevice device;
        private RenderView view;

        [Id(1)]
        public List<WindowRenderable> Renderables;

        internal Window()
        {
            Renderables = new List<WindowRenderable>();
        }

        internal void Init(Flood.Window window, RenderDevice renderDevice)
        {
            device = renderDevice;

            var context = window.CreateContext(new RenderContextSettings());

            view = window.CreateView();
            view.ClearColor = Color.Red;

            window.Idle += Update;
            window.Render += Render;
            window.TargetResize += Resize;
            window.Show(true);

            window.MakeCurrent();
            context.Init();
        }

        private void Update()
        {
            foreach (var appRenderable in Renderables)
                appRenderable.Update();
        }

        public void Render()
        {
            var rb = new RenderBlock();

            foreach (var appRenderable in Renderables)
                appRenderable.Render(rb);

            device.ActiveView = view;
            device.ClearView();

            device.Render(rb);
            //Application.Instance.Engine.StepFrame();
        }

        public void Resize(Settings settings)
        {
            foreach (var appRenderable in Renderables)
                appRenderable.Resize(settings.Width, settings.Height);
        }
    }
}
