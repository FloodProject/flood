using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Flood.GUI.Controls;
using Mono.Addins;

namespace Editor.Client.GUI
{
    /// <summary>
    /// 
    /// </summary>
    [TypeExtensionPoint]
    public class Pane 
    {
        public string Title = "Pane";

        public virtual Control CreateControl(Control parent)
        {
            return new Button(parent);
        }
    }
}
