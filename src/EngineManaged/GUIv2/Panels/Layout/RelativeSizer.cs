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
                var position = ((RelativePanel)Panel).ChildrenPositions[control];
                var w = Panel.Width - position.X - control.Margin.Left;
                var h = Panel.Height - position.Y - control.Margin.Top;
                if (w <= 0 || h <= 0)
                    w = h = 1;
                PositionAndAlign(position, new Vector2i(w, h), control);
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
