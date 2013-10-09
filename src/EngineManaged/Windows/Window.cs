﻿
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

        public virtual void Resize(int width, int height)
        {
        }
    }

    [DataObject(5)]
    public class Window
    {
        private Flood.Window nativeWindow;
        private RenderDevice device;
        private RenderView view;
        internal RenderContext RenderContext;

        [Id(1)]
        public List<WindowRenderable> Renderables;

        internal Window()
        {
            Renderables = new List<WindowRenderable>();
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
            view.ClearColor = Color.Red;

            window.Idle += Update;
            window.Render += Render;
            window.TargetResize += Resize;
            window.Show(true);
        }

        private void Update()
        {
            nativeWindow.Update();
        }

        internal void Render()
        {
            var rb = new RenderBlock();

            foreach (var appRenderable in Renderables)
                appRenderable.Render(rb);

            RenderContext.MakeCurrent(nativeWindow);

            device.ActiveView = view;
            device.ClearView();

            device.Render(rb);
        }

        public void Resize(Settings settings)
        {
            foreach (var appRenderable in Renderables)
                appRenderable.Resize(settings.Width, settings.Height);
        }
    }
}
