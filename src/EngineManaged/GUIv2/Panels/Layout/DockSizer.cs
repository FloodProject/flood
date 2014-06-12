using System;
using System.Collections.Generic;

namespace Flood.GUIv2.Panels.Layout
{
    public class DockSizer : Sizer
    {

        public DockSizer(DockPanel panel) : base(panel)
        {
        }

        public override void RecalcSizes()
        {
            foreach (var control in Children)
            {
                if (control.IsHidden)
                    continue;
                PositionAndAlign(new Vector2i(0, 0), new Vector2i(Panel.Width, Panel.Height), control);
            }
        }

    }
}
