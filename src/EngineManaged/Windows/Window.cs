
using Flood.Remoting.Metadata;
using System.Collections.Generic;

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

        public virtual void Resize(int width, int height)
        {
        }
    }

    [DataObject(5)]
    public class Window
    {
        public Flood.Window nativeWindow;

        internal RenderDevice device;
        internal RenderView view;
        internal RenderContext RenderContext;

        [Id(1)]
        private List<WindowRenderable> renderables;

        internal Window()
        {
            renderables = new List<WindowRenderable>();
        }

        internal void Init(Flood.Window window, RenderDevice renderDevice, RenderContext renderContext)
        {
            nativeWindow = window;
            device = renderDevice;

            if (renderContext == null)
            {
                RenderContext = window.CreateContext(new RenderContextSettings());
                window.MakeCurrent();
                RenderContext.Init();
            }
            else
            {
                RenderContext = renderContext;
                nativeWindow.Context = RenderContext;
            }

            view = window.CreateView();

            window.Render += Render;
            window.TargetResize += Resize;
        }

        public void AddRenderable(WindowRenderable renderable)
        {
            renderable.Resize(nativeWindow.Settings.Width, nativeWindow.Settings.Height);
            renderables.Add(renderable);
        }

        internal void Render()
        {
            var rb = new RenderBlock();

            foreach (var appRenderable in renderables)
                appRenderable.Render(rb);

            RenderContext.MakeCurrent(nativeWindow);

            device.ActiveView = view;
            device.ClearView();

            device.Render(rb);

            nativeWindow.Update();
        }

        private void Resize(Settings settings)
        {
            foreach (var appRenderable in renderables)
                appRenderable.Resize(settings.Width, settings.Height);
        }
    }
}
