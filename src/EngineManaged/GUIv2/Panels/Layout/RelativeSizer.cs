using System;

namespace Flood.GUIv2.Panels.Layout
{
    public class RelativeSizer : Sizer
    {
        public RelativeSizer(RelativePanel panel) : base(panel)
        {

        }

        public override void RecalcSizes()
        {
            foreach (var control in Children)
            {
                if (control.IsHidden)
                    continue;
                var position = ((RelativePanel)Panel).ChildrenPositions[control] - new Vector2i(control.Margin.Left, control.Margin.Top);
                position.X = Math.Max(0, position.X);
                position.Y = Math.Max(0, position.Y);
                var w = Panel.Width - position.X;
                var h = Panel.Height - position.Y;
                if (w <= 0 || h <= 0)
                    w = h = 1;
                PositionAndAlign(position, new Vector2i(w, h), control);
            }
        }

    }
}
