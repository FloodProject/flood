using Editor.Client.GUI;
using Editor.Client.GUI.Controls;
using Flood.GUI;

namespace Editor.Client.Gui
{
    /// <summary>
    /// Window composed by a MenuBar a Container.
    /// </summary>
    public class MainWindow : Window
    {
        private MenuBar menuBar;

        private ToolBarContainer toolBarContainer;

        protected override void Init()
        {
            menuBar = new MenuBar(Canvas,"/Flood/Editor/Client/Menubar");
            toolBarContainer = new ToolBarContainer(Canvas,"/Flood/Editor/Client/Toolbar");

            toolBarContainer.Dock = Pos.Fill;

            PaneManager.AddPane(new Pane());
            PaneManager.AddPane(new Pane());
            PaneManager.AddPane(new Pane());
            PaneManager.AddPane(new Pane());
        }
    }
}
