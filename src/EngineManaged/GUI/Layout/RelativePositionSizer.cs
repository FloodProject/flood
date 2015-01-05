using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Flood.GUI.Controls.Containers;

namespace Flood.GUI
{
    public class RelativePositionSizer : Sizer
    {
        public RelativePositionSizer(RelativePositionPanel panel) : base(panel)
        {
        }

        public override void RecalcSizes()
        {
            var size = GetPanelSize();
            Panel.Width = size.X;
            Panel.Height = size.Y;
            foreach (var control in Children)
            {
                var dims = control.GetSizeWithBorder();
                var position = ((RelativePositionPanel) Panel).ChildrenPositions[control];
                if(position.X > Panel.Width || position.Y > Panel.Height)
                {
                    control.Hide();
                    control.Disable();
                    continue;
                }
                if(position.X + dims.X > Panel.Width)
                    control.Width -= position.X + dims.X - Panel.Width;
                if(position.Y + dims.Y > Panel.Height)
                    control.Height -= position.Y + dims.Y - Panel.Height;

                control.X = position.X;
                control.Y = position.Y;
            }
        }
        private Vector2i GetPanelSize()
        {
            switch (Panel.Expansion)
            {
                case ExpansionFlags.Expand:
                    return GetPanelSizeExpand();
                case ExpansionFlags.Shaped:
                    return GetPanelSizeShaped();
                default:
                    return GetPanelSizeFit();
            }
        }

        private Vector2i GetPanelSizeFit()
        {
            var width = 0;
            var height = 0;
            foreach (var control in Children)
            {
                var dims = control.GetSizeWithBorder(); 
                var position = ((RelativePositionPanel) Panel).ChildrenPositions[control];
                width = Math.Max(position.X + dims.X, width);
                height = Math.Max(position.Y + dims.Y, height);
            }
            return new Vector2i(width, height);
        }

        private Vector2i GetPanelSizeShaped()
        {
            return new Vector2i(Panel.Width, Panel.Height);
        }

        private Vector2i GetPanelSizeExpand()
        {
            if (Panel.Parent == null)
                throw new NullParentException();
            return new Vector2i(Panel.Parent.Width, Panel.Parent.Height);
        }

        public override Vector2i CalcMin()
        {
            foreach (var control in Panel.Children)
                control.ReduceToMinSize();
            RecalcSizes();
            return new Vector2i(Panel.Width, Panel.Height);
        }

        public override bool InformFirstDirection(BoxOrientation direction, int size, int availableOtherDir)
        {
            var PrevMin = CalcMin();
            foreach (var control in Children)
                control.InformFirstDirection(direction, size, availableOtherDir);
            var Min = CalcMin();
            return Min.AnyLT(PrevMin);
        }
    }
}
