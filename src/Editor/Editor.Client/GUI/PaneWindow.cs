using System;
using Flood.Editor.Client.GUI;

namespace Flood.Editor.Client.Gui
{
    /// <summary>
    /// Window composed exclusively by a Container.
    /// </summary>
    public class PaneWindow : GuiWindow
    {
        public Container Container { get; private set; }

        public PaneWindow()
        {
            throw new NotImplementedException();
        }

        protected override void Init()
        {
            throw new NotImplementedException();
        }

        static public Window Create(RenderContext context)
        {
            var engine = FloodEngine.GetEngine();
            var windowManager = engine.WindowManager;

            var settings = new WindowSettings
            {
                Width = 640,
                Height = 480,
                Title = "Pane",
                Styles = WindowStyles.TopLevel
            };

            var window = windowManager.CreateWindow(settings);
            window.WindowClose += () => Log.Info("Window closed!");
            window.WindowFocusChange += hasFocus => Log.Info("Window focus: " + hasFocus);
            window.TargetResize += s => Log.Info("Size: " + s.Width);
            window.Show(visible: true);

            window.Context = context;
            return window;
        }
    }
}
