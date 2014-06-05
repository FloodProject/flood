using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Flood.GUIv2.Panels.Layout
{
    public class BoxSizer : Sizer
    {
        class BoxLayout
        {
            private Orientation orientation;
            public BoxLayout(Orientation orientation)
            {
                this.orientation = orientation;
            }
            public List<Cell> Elements = new List<Cell>();
            private int m_Width;
            public int Width { get { return m_Width; } }
            private int m_Height;
            public int Height { get { return m_Height; } }
            public int MajorDirection
            {
                get { return orientation == Orientation.Vertical ? m_Height : m_Width; }
                set
                {
                    if (orientation == Orientation.Vertical)
                        m_Height = value;
                    else
                        m_Width = value;
                }
            }
            public int MinorDirection
            {
                get { return orientation == Orientation.Vertical ? m_Width : m_Height; }
                set
                {
                    if (orientation == Orientation.Vertical)
                        m_Width = value;
                    else
                        m_Height = value;
                }
            }
        }

        struct Cell
        {
            public Cell(int majorPos, int minorPos, int majorDim,int minorDim, IControl control, Orientation orientation)
            {
                X = (orientation == Orientation.Vertical)? minorPos : majorPos;
                Y = (orientation == Orientation.Vertical)? majorPos : minorPos;
                Width = (orientation == Orientation.Vertical)? minorDim : majorDim;
                Height = (orientation == Orientation.Vertical)? majorDim : minorDim;
                Control = control;
            }
            public readonly int X;
            public readonly int Y;
            public readonly int Width;
            public readonly int Height;
            public readonly IControl Control;

        }

        private BoxPanel BoxPanel;
        /// <summary>
        /// Initializes a new instance of the <see cref="BoxSizerOld"/> class.
        /// </summary>
        /// <param name="panel">The panel.</param>
        public BoxSizer(BoxPanel panel)
            : base(panel)
        {
            BoxPanel = panel;
            Debug.Assert(panel.Orientation == Orientation.Vertical
                        || panel.Orientation == Orientation.Horizontal
                        , "Invalid value for BoxSizerOld orientation");
        }

        public override void RecalcSizes()
        {
            var width = Panel.Width;
            var height = Panel.Height;

            var freeMajorSpace = MajorDirection(Panel.Width - Panel.LayoutMinSize.X,
                Panel.Height - Panel.LayoutMinSize.Y);

            //not enoug space, satisfy as many mins as possible by order of insertion
            if(freeMajorSpace < 0)
            {
                AssignMinSizes(width, height);
                return;
            }

            var minorDirection = MinorDirection(width, height);
            var controlBoxes = new Dictionary<IControl, Vector2i>();
            //assign as many fixed best sizes as possible
            foreach (var control in Children)
            {
                if (control.IsHidden) 
                    continue;
                var majorBestSize = MajorDirection(control.LayoutBestSize);
                var majorMinSize = MajorDirection(control.LayoutMinSize);

                var currentOverflow = 0;
                if (majorBestSize  != Control.MinNotSet)
                    currentOverflow = majorBestSize - majorMinSize;

                if(currentOverflow <= freeMajorSpace)
                {
                    freeMajorSpace -= currentOverflow;
                    controlBoxes[control] = GenBox(majorBestSize, minorDirection);
                }
                else
                    controlBoxes[control] = GenBox(majorMinSize, minorDirection);
            }

            var freeSpace = freeMajorSpace;

            var expandNotMaxed = Children.Where(child => 
                {
                    if(child.IsHidden || !IsExpandInMajorDiretion(child))
                        return false;
                    return IsMajorVertical()
                                ? controlBoxes[child].Y < child.LayoutMaxSize.Y
                                : controlBoxes[child].X < child.LayoutMaxSize.X;
                }).ToList();

            while(freeSpace > 0 && expandNotMaxed.Count > 0)
            {
                var remainder = 0;
                var controlExtraSpace = freeSpace / expandNotMaxed.Count;
                if (controlExtraSpace == 0)
                    break;
                foreach (var control in expandNotMaxed)
                {
                    var curMajor = IsMajorVertical() 
                        ? controlBoxes[control].Y
                        : controlBoxes[control].X;
                    var majorMax = MajorDirection(control.LayoutMaxSize);
                    Debug.Assert(majorMax != Control.MinNotSet);

                    var major = Math.Min(curMajor + controlExtraSpace, majorMax);
                    controlBoxes[control] = GenBox(major, minorDirection);
                    Debug.Assert(major >= curMajor);
                    var diff = major - curMajor;
                    remainder += (controlExtraSpace > diff) ? controlExtraSpace - diff : 0;
                }
                freeSpace = remainder;
                expandNotMaxed = Children.Where(child => !child.IsHidden && IsExpandInMajorDiretion(child)
                    && (IsMajorVertical())
                    ? controlBoxes[child].Y < child.LayoutMaxSize.Y
                    : controlBoxes[child].X < child.LayoutMaxSize.X).ToList();
            }

            if (freeSpace > 0)
            {
                var controls = Children.Where(child => child.IsHidden).ToList();
                var controlExtraSpace = freeSpace / controls.Count;
                //distribute remaining space evenly along all controls
                for (int i = 0; i < controls.Count; i++)
                {
                    var control = controls[i];
                    var curMajor = IsMajorVertical()
                                ? controlBoxes[control].Y
                                : controlBoxes[control].X;

                    controlBoxes[control] = GenBox(curMajor + controlExtraSpace, minorDirection);
                }

                //if any left over add it to last control
                var finalMajor = IsMajorVertical() ? controlBoxes[controls.Last()].Y 
                                                : controlBoxes[controls.Last()].X;
                controlBoxes[controls.Last()] = GenBox(finalMajor + freeSpace - controlExtraSpace * controls.Count, minorDirection);
            }

            //position controls
            var majorCounter = 0;
            foreach (var control in Children)
            {
                if (control.IsHidden) 
                    continue;
                var size = controlBoxes[control];
                var x = IsMajorVertical() ? 0 : majorCounter;
                var y = IsMajorVertical() ? majorCounter : 0;

                PositionAndAlign(new Vector2i(x, y), size, control);
                
                majorCounter += IsMajorVertical() ? size.Y : size.X;
            }
        }
        private bool IsExpandInMajorDiretion(IControl control)
        {
            return IsMajorVertical() ? control.IsExpandVertical || control.IsFillVertical : control.IsExpandHorizontal || control.IsFillHorizontal;
        }

        private Vector2i GenBox(int majorDirection, int minorDirection)
        {
            return IsMajorVertical()
                       ? new Vector2i(minorDirection, majorDirection)
                       : new Vector2i(majorDirection, minorDirection);
        }

        private void AssignMinSizes(int width, int height)
        {
            var majorCounter = 0;
            foreach (var control in Children)
            {
                if (control.IsHidden)
                    continue;

                var majorDirection = MajorDirection(control.LayoutMinSize.X, control.LayoutMinSize.Y);
                var minorDirection = MinorDirection(width, height);
                if (majorDirection == Control.MinNotSet) 
                    continue;
                majorCounter += majorDirection;
                var x = IsMajorVertical() ? 0 : majorCounter;
                var y = IsMajorVertical() ? majorCounter : 0;

                PositionAndAlign(new Vector2i(x, y), new Vector2i(majorDirection, minorDirection), control);
            }
        }


        public override Vector2i CalcMin()
        {
            throw new NotImplementedException();
        }

        private bool IsMajorVertical()
        {
            return BoxPanel.Orientation == Orientation.Vertical;
        }

        public int MajorDirection(int x, int y)
        {
            return IsMajorVertical()? y : x;
        }

        public int MinorDirection(int x, int y)
        {
            return IsMajorVertical()? x : y;
        }

        public int MajorDirection(Vector2i size)
        {
            return IsMajorVertical() ? size.Y : size.X;
        }

        public int MinorDirection(Vector2i size)
        {
            return IsMajorVertical() ? size.X : size.Y;
        }

    }
}

