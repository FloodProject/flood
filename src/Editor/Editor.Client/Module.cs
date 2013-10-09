
using Editor.Services;
using Flood.Editor.Client.GUI;
using Flood.Modules;
using Flood.Windows;
using GUI;
using GUI.Controls;

namespace Editor.Client
{
    class Module : IModule
    {
        public void OnLoad(Flood.Remoting.ServiceManager serviceManager)
        {
            var windowManager = serviceManager.GetGlobalService<IWindowManager>();

            var editorWindowTask = windowManager.GetCreateWindow("Editor");
            editorWindowTask.Wait();
            var editorWindow = editorWindowTask.Result;

            var gui = new GuiRenderable();

            editorWindow.AddRenderable(gui);

            var container = new Container(gui.Canvas);
            
            var paneGroup = new PaneGroup(container);
            container.InsertPanel(paneGroup);

            paneGroup.AddPane(new Pane{Title = "Pane1"});
            paneGroup.AddPane(new Pane{Title = "Pane2"});
        }

        public void Update()
        {
            throw new System.NotImplementedException();
        }
    }
}
