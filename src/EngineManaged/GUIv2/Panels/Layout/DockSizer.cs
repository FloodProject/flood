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

        public override Vector2i CalcMin()
        {
            foreach (var control in Children)
                control.ReduceToMinSize();
            RecalcSizes();
            return new Vector2i(Panel.Width, Panel.Height);
        }
    }
}
