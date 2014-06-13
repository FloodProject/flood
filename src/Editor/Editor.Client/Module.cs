using Flood.Editor.Services;
using Flood.GUI;
using Flood.Modules;
using Flood.Remoting;
using Flood.Windows;

namespace Editor.Client
{
    class Module : IModule
    {
        public void OnLoad(ServiceManager serviceManager)
        {
            var windowManager = serviceManager.GetGlobalService<IWindowManager>();
            var editorWindowTask = windowManager.GetCreateWindow("Editor");
            editorWindowTask.Wait();
            var editorWindow = editorWindowTask.Result;

            var gui = new GuiRenderable();
            editorWindow.AddRenderable(gui);

            var paneManager = new PaneManager(gui.Canvas);
            serviceManager.GetCreateImplementation<IPaneManager>(paneManager);

            paneManager.AddPane(new Pane {Title = "Pane1"});
            paneManager.AddPane(new Pane {Title = "Pane2"});
        }

        public void Update()
        {
            throw new System.NotImplementedException();
        }
    }
}
