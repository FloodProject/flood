using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Flood.GUI;
using Flood.GUI.Controls;

namespace Flood.Editor.Client.GUI.Controls
{
    /// <summary>
    /// Container that can have toolbars assigned to its outer bounds.
    /// </summary>
    public class ToolBarContainer : Control
    {
        private string extensionPoint;

        private ToolBar topToolbar;
        private Container container;

        public ToolBarContainer(Control parent, string extensionPoint) : base(parent)
        {
            this.extensionPoint = extensionPoint;
            topToolbar = new ToolBar(this,extensionPoint);
            topToolbar.Dock = Pos.Top;
            container = new Container(this);
            container.Dock = Pos.Fill;
        }
    }
}
