using System;
using System.Collections.Generic;
using System.ComponentModel.Composition;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Flood.Editor;
using Flood.Editor.Controls;
using Flood.GUI.Controls;

namespace Editor.Client.GUI
{
    /// <summary>
    /// 
    /// </summary>
    [Export]
    public class ProjectPaneMenu : EditorTool, DockableTool
    {
        [Import] 
        private ProjectPane projectPane;

        private Flood.GUI.Controls.Menu menu; 

        public void InitControl(Flood.GUI.Controls.Control parent)
        {
            menu = new Menu(parent);
        }


        public void Show(Project project)
        {
            
        }
    }
}
