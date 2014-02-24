using Flood.Remoting.Metadata;
using System.Collections.Generic;
using System.Threading.Tasks;

namespace Flood.Windows
{
    [GlobalService(1)]
    public interface IWindowManager
    {
        [Id(1)]
        Task<Window> GetCreateWindow([Id(1)]string windowName);
    }

    public class WindowManager : IWindowManager
    {
        private Dictionary<string, Window> windows;
        private Application application;
        private RenderContext renderContext;

        public WindowManager(Application application)
        {
            this.application = application;
            windows = new Dictionary<string, Window>();
        }

        public Window GetWindow(string windowName)
        {
            return windows.ContainsKey(windowName) ?
                windows[windowName] : null;
        }

        public async Task<Window> GetCreateWindow(string windowName)
        {
            if (windows.ContainsKey(windowName))
                return windows[windowName];

            var window = new Window();

            var windowSettings = new WindowSettings(1000, 700, windowName, WindowStyles.TopLevel);
            var nativeWindow = application.NativeWindowManager.CreateWindow(windowSettings);
            window.Init(nativeWindow, application.RenderDevice, renderContext);

            if (windows.Count == 0)
                renderContext = window.RenderContext;

            windows.Add(windowName, window);
            return window;
        }

        internal void Render()
        {
            foreach (var window in windows.Values)
                window.Render();
        }
    }
}
