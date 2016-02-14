using Flood.Editor.Services;
using Flood.GUI;
using Flood.Modules;
using Flood.Remoting;
using Flood;
using Flood.Windows;
using Menu = Flood.Menu;
using MenuItemKind = Flood.MenuItemKind;

namespace Editor.Client
{
    class Module : IModule
    {
        public Menu fileMenu;
        public MenuItem openItem;

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

            paneManager.AddPane(new Pane { Title = "Pane1" });
            paneManager.AddPane(new Pane { Title = "Pane2" });

            var application = serviceManager.Application;

            var nativeWindowManager = application.Engine.WindowManager;
            fileMenu = nativeWindowManager.CreateMenu();
            openItem = fileMenu.AppendItem("Open", MenuItemKind.Normal);
            openItem.Selected +=
                () => nativeWindowManager.OpenFileDialog("*", FileDialogFlags.Open);
            fileMenu.AppendItem("Close", MenuItemKind.Normal);
            fileMenu.AppendItem("New", MenuItemKind.Normal);

            var menuBar = nativeWindowManager.CreateMenuBar();
            menuBar.Append(fileMenu, "File");

            var nativeWindow = application.WindowManager.GetWindow("Editor");
            nativeWindow.nativeWindow.MenuBar = menuBar;
            nativeWindow.nativeWindow.Show(true);
        }

        public void Update()
        {
            throw new System.NotImplementedException();
        }
    }
}
