using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Flood.GUIv2.Panels.Layout
{
    public class DockSizer : Sizer
    {

        public DockSizer(DockPanel panel) : base(panel)
        {
        }

        public override void RecalcSizes()
        {
            SetPanelSize();
            switch (Panel.Expansion)
            {
                case ExpansionFlags.Fit:
                    PositionControlsFit();
                    break;
                default:
                    PositionControlsFixedSize();
                    break;
            }
        }

        private void PositionControlsFit()
        {
            foreach (var control in Children)
            {
                var position = new Vector2i(control.Margin.Left, control.Margin.Top);
                if ((control.Alignment & AlignmentFlags.Right) != 0)
                    position.X = Panel.Width - control.Width - control.Margin.Right;
                if ((control.Alignment & AlignmentFlags.Bottom) != 0)
                    position.Y = Panel.Height - control.Height - control.Margin.Bottom;
                if ((control.Alignment & AlignmentFlags.CenterHorizontal) != 0)
                    position.X = (Panel.Width - control.RenderWidth)/2;
                if ((control.Alignment & AlignmentFlags.CenterVertical) != 0)
                    position.Y = (Panel.Height - control.RenderHeight)/2;

                control.SetDimension(position, new Vector2i(control.Width, control.Height));
            }
        }

        private void PositionControlsFixedSize()
        {
            foreach (var control in Children)
            {
                if(control.RenderWidth > Panel.Width || control.RenderHeight > Panel.Height)
                    control.ReduceToMinSize();
                var position = new Vector2i(Math.Min(control.Margin.Left, Panel.Width), Math.Min(control.Margin.Top, Panel.Height));
                if ((control.Alignment & AlignmentFlags.Right) != 0)
                    position.X = Math.Max((Panel.Width - control.Width - control.Margin.Right), position.X);
                if ((control.Alignment & AlignmentFlags.Bottom) != 0)
                    position.Y = Math.Max((Panel.Height - control.Height - control.Margin.Bottom), Position.Y);
                if ((control.Alignment & AlignmentFlags.CenterHorizontal) != 0)
                    position.X = Math.Max((Panel.Width - control.RenderWidth)/2, position.X);
                if ((control.Alignment & AlignmentFlags.CenterVertical) != 0)
                    position.Y = Math.Max((Panel.Height - control.RenderHeight)/2, position.Y);

                var controlWidth = Math.Max(Math.Min(control.Width, Panel.Width - control.Margin.Left - control.Margin.Right), 0);
                var controlHeight = Math.Max(Math.Min(control.Height, Panel.Height - control.Margin.Top - control.Margin.Bottom), 0);
                
                control.SetDimension(position, new Vector2i(controlWidth, controlHeight));
            }
        }

        private void SetPanelSize()
        {
            switch (Panel.Expansion)
            {
                case ExpansionFlags.Fit:
                    var dims = GetDimensions();
                    Panel.Width = dims.Item1;
                    Panel.Height = dims.Item2;
                    break;
                case ExpansionFlags.Expand:
                    var parent = Panel.LogicParent;
                    if (parent == null)
                        throw new Exception("DockSizer: Parent is Null");
                    Panel.Width = parent.Width;
                    Panel.Height = parent.Height;
                    break;
            }
        }

        private Tuple<int, int> GetDimensions()
        {
            int widthL = 0, widthR = 0, widthV = 0;
            int heightB = 0, heightT = 0,heightH = 0;
            foreach (var control in Children)
            {
                if ((control.Alignment & AlignmentFlags.Right) != 0)
                {
                    widthR = Math.Max(control.RenderWidth, widthR);
                    heightH = Math.Max(control.RenderHeight, heightH);
                }
                if ((control.Alignment & AlignmentFlags.Left) != 0)
                {
                    widthL = Math.Max(control.RenderWidth, widthL);
                    heightH = Math.Max(control.RenderHeight, heightH);
                }
                if ((control.Alignment & AlignmentFlags.Top) != 0)
                {
                    widthV = Math.Max(control.RenderWidth, widthV);
                    heightT = Math.Max(control.RenderHeight, heightT);
                }
                if ((control.Alignment & AlignmentFlags.Bottom) != 0)
                {
                    widthV = Math.Max(control.RenderWidth, widthV);
                    heightB = Math.Max(control.RenderHeight, heightB);
                }
                if((control.Alignment & (AlignmentFlags.CenterVertical | AlignmentFlags.CenterHorizontal | AlignmentFlags.NotSet)) != 0)
                {
                    widthV = Math.Max(control.RenderWidth, widthV);
                    heightH = Math.Max(control.RenderHeight, heightH);
                }
            }

            return new Tuple<int, int>(Math.Max(widthL + widthR, widthV), Math.Max(heightB + heightT, heightH));
        }

        public override Vector2i CalcMin()
        {
            foreach (var control in Children)
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
            return Min < PrevMin;
        }
    }
}
