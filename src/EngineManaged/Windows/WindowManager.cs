
using System.Collections.Generic;
using System.Threading.Tasks;
using Flood.Remoting.Metadata;

namespace Flood.Windows
{
    [GlobalService(1)]
    public interface IWindowManager
    {
        [Id(1)]
        Task<Window> GetCreateWindow([Id(1)]string windowName);
    }

    class WindowManager : IWindowManager
    {
        private Dictionary<string, Window> windows;
        private Application application;

        public WindowManager(Application application)
        {
            this.application = application;
            windows = new Dictionary<string, Window>();
        }

        public async Task<Window> GetCreateWindow(string windowName)
        {
            if (windows.ContainsKey(windowName))
                return windows[windowName];

            var window = new Window();
            windows.Add(windowName, window);

            var windowSettings = new WindowSettings(1000, 700, windowName, WindowStyles.TopLevel);
            var nativeWindow = application.NativeWindowManager.CreateWindow(windowSettings);
            window.Init(nativeWindow, application.RenderDevice);

            return window;
        }
    }
}
