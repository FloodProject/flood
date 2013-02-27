using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Editor.Client.GUI;
using Editor.Client.GUI.Controls;
using Flood.GUI;
using Flood.GUI.Controls;

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
        }
    }
}
