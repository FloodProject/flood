using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Flood.GUIv2.Panels.Layout;

namespace Flood.GUIv2.Panels
{
    public class DockPanel : Panel
    {
        public DockPanel(ExpansionFlags expansion = ExpansionFlags.Fit) 
            : base(expansion)
        {
            new DockSizer(this);
        }
    }
}
