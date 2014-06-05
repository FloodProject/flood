using System.Linq;
using Flood.GUIv2.Panels.Layout;

namespace Flood.GUIv2.Panels
{
    public class BoxPanelOld : Panel
    {
        /// <summary>
        /// The used space in the major direction
        /// </summary>
        private int UsedMajorDirection;

        private Orientation m_orientation;

        /// <summary>
        /// Gets or sets the box panel orientation.
        /// </summary>
        /// <remarks>
        /// If orientation is altered the pane is laid out again
        /// </remarks>
        public Orientation Orientation
        {
            get
            {
                return m_orientation;
            }
            set 
            {
                if (m_orientation == value)
                    return;
                m_orientation = value;
                if(Children.Count == 0)
                    return;
                ResetOrientation();
                Invalidate();
            }
        }

        /// <summary>
        /// Initializes a new instance of the <see cref="BoxPanel"/> class.
        /// </summary>
        /// <param name="orientation">The orientation of the box panel.</param>
        /// <param name="expansion">The type of <see cref="ExpansionFlags"/> of this panel.</param>
        public BoxPanelOld(Orientation orientation = Orientation.Vertical, ExpansionFlags expansion = ExpansionFlags.Fit)
            : base(expansion)
        {
            Orientation = orientation;
            if (Expansion == ExpansionFlags.Fit)
            {
                Width = 0;
                Height = 0;
            }
            new BoxSizerOld(this);
        }

        /// <summary>
        /// Adds a child control to the panel.
        /// </summary>
        /// <param name="control">The child control.</param>
        public override void AddChild(IControl control)
        {
            base.AddChild(control);
            AddControl(control);
        }

        /// <summary>
        /// Adhusts panel's dimensions based on control added and type of panel expansion.
        /// </summary>
        /// <param name="control">The control added.</param>
        private void AddControl(IControl control)
        {
            switch (Expansion)
            {
                case ExpansionFlags.Fit:
                    AddControlFit(control);
                    break;
                case  ExpansionFlags.Expand:
                    AddControlExpand(control);
                    break;
            }
        }

        /// <summary>
        /// Adjusts panel's dimensions when a new control is added for <see cref="ExpansionFlags"/> type Fit.
        /// </summary>
        /// <param name="control">The control added.</param>
        private void AddControlFit(IControl control)
        {
            if (control.IsHidden)
                return;

            if (Orientation == Orientation.Vertical)
            {
                Width = (control.RenderWidth > Width) ? control.RenderWidth : Width;
                Height += control.RenderHeight;
            }
            else
            {
                Height = (control.RenderHeight > Height) ? control.RenderHeight : Height;
                Width += control.RenderWidth;
            }
            VerifyBounds();
            Invalidate();
        }

        /// <summary>
        /// Adjusts panel's dimensions when a new control is added for <see cref="ExpansionFlags"/> type Expand.
        /// </summary>
        /// <param name="control">The control added.</param>
        private void AddControlExpand(IControl control)
        {
            if (control.IsHidden)
                return;

            if (Orientation == Orientation.Vertical)
            {
                Width = (control.RenderWidth > Width) ? control.RenderWidth : Width;
                UsedMajorDirection += control.RenderHeight;
                Height = (UsedMajorDirection > Height) ? UsedMajorDirection : Height;
            }
            else
            {
                Height = (control.RenderHeight > Height) ? control.RenderHeight : Height;
                UsedMajorDirection += control.RenderWidth;
                Width = (UsedMajorDirection > Width) ? UsedMajorDirection : Width;
            }
            VerifyBounds();
            Invalidate();
        }

        /// <summary>
        /// Detaches specified control from this one.
        /// </summary>
        /// <param name="child">Child to be removed.</param>
        /// <param name="dispose">Determines whether the child should be disposed (added to delayed delete queue).</param>
        public override void RemoveChild(IControl child, bool dispose)
        {
            base.RemoveChild(child, dispose);
            if(!dispose)
                Trim(child);
            PreLayout(Skin);
        }

        public override void LayoutSizes()
        {
            throw new System.NotImplementedException();
        }

        /// <summary>
        /// Resizes panel when  a control is removed and the expansion type is Fit.
        /// </summary>
        /// <param name="control">The removed control.</param>
        private void Trim(IControl control)
        {
            if (control.IsHidden || Expansion != ExpansionFlags.Fit)
                return;

            if (Children.Count == 0)
            {
                Width = 0;
                Height = 0;
            }
            else
            {
                if (Orientation == Orientation.Vertical)
                {
                    Width = (control.RenderWidth == Width)? 
                        Children.OrderByDescending(x => x.RenderWidth).FirstOrDefault().RenderWidth : Width;
                    Height -= control.RenderHeight;
                }
                else
                {
                    Height = (control.RenderHeight == Height) ? 
                        Children.OrderByDescending(x => x.RenderHeight).FirstOrDefault().RenderHeight : Height;
                    Width -= control.RenderWidth;
                }
            }
            VerifyBounds();
            Invalidate();
        }

        /// <summary>
        /// Detaches specified control from this one.
        /// </summary>
        /// <param name="index"> Index of the child to be removed.</param>
        /// <param name="dispose">Determines whether the child should be disposed (added to delayed delete queue).</param>
        public void RemoveChild(int index, bool dispose)
        {
            if (!ValidIndex(index))
                return;
            RemoveChild(Children[index], dispose);
        }


        /// <summary>
        /// Swaps the position of two children.
        /// </summary>
        /// <param name="first">Index of the first control to be swapped.</param>
        /// <param name="second">Index of the second control to be swapped.</param>
        public void SwapChildren(int first, int second)
        {

            if (!ValidIndex(first) || !ValidIndex(second))
                return;
            var firstControl = Children[first];
            var secondControl = Children[second];
            base.RemoveChild(firstControl, false);
            base.InsertChild(firstControl, second);
            base.RemoveChild(secondControl, false);
            base.InsertChild(secondControl, first);
            PreLayout(Skin);
        }

        /// <summary>
        /// Validates a child index.
        /// </summary>
        /// <param name="index">The index of the child control.</param>
        /// <returns>Whether index is contained within the children list's bounds.</returns>
        private bool ValidIndex(int index)
        {
            return index >= 0 && index < Children.Count;
        }

        /// <summary>
        /// Clears the panel.
        /// </summary>
        public void Clear()
        {
            DeleteAllChildren();
            PreLayout(Skin);
        }

        /// <summary>
        /// Verifies if max or min bounds have been crossed and adjusts accordingly.
        /// </summary>
        private void VerifyBounds()
        {
            //check if bounds are set and if so if the values exceed them and adjust accordingly
            if(MaximumSize.X != MaxNotSet && Width > MaximumSize.X)
                Width = MaximumSize.X;
            else if (MinimumSize.X != MaxNotSet && Width < MinimumSize.X)
                Width = MinimumSize.X;

            if (MaximumSize.Y != MaxNotSet && Height > MaximumSize.Y)
                Height = MaximumSize.Y;
            else if (MinimumSize.Y != MaxNotSet && Height < MinimumSize.Y)
                Height = MinimumSize.Y;
        }

        /// <summary>
        /// Resets width and height according to current orientation.
        /// </summary>
        public void ResetOrientation()
        {
            if (Expansion == ExpansionFlags.Shaped)
                return;
            Redimension();
        }

        //public override void Layout(Skins.Skin skin)
        //{
        //    Redimension();
        //    base.Layout(skin);
        //}

        /// <summary>
        /// Redimensions the panel according to expansion type, orientation and children.
        /// </summary>
        public void Redimension()
        {
            if(Expansion == ExpansionFlags.Fit)
            {
                Width = 0;
                Height = 0;
            }
            ResetOrientationFunction resFunc;
            if (Orientation == Orientation.Horizontal)
            {
                switch (Expansion)
                {
                    case ExpansionFlags.Fit:
                        resFunc = ResetHorizontalFit;
                        break;
                    default: //ExpansionFlags.Expand
                        resFunc = ResetHorizontalExpand;
                        break;
                }
            }
            else
            {
                switch (Expansion)
                {
                    case ExpansionFlags.Fit:
                        resFunc = ResetVerticalFit;
                        break;
                    default: //ExpansionFlags.Expand
                        resFunc = ResetVerticalExpand;
                        break;
                }
            }

            UsedMajorDirection = 0;
            foreach (var control in Children)
                resFunc(control);
            VerifyBounds();
            Invalidate();
        }

        /// <summary>
        /// Delegate to help resize panel based on orientation and expansion type.
        /// </summary>
        /// <param name="control">The control used in the intermediate step.</param>
        private delegate void ResetOrientationFunction(IControl control);

        /// <summary>
        /// Calculates intermediate size if Orientation is horizontal and Expansion is Fit
        /// </summary>
        /// <param name="ctrl">The control used to calculate the intermediate size.</param>
        private void ResetHorizontalFit(IControl ctrl)
        {
            if (ctrl.IsHidden)
                return;
            Height = (ctrl.RenderHeight > Height) ? ctrl.RenderHeight : Height;
            Width += ctrl.RenderWidth;
        }

        /// <summary>
        /// Calculates intermediate size if Orientation is vertical and Expansion is Fit
        /// </summary>
        /// <param name="ctrl">The control used to calculate the intermediate size.</param>

        private void ResetVerticalFit(IControl ctrl)
        {
            if (ctrl.IsHidden)
                return;
            Width = (ctrl.RenderWidth > Width) ? ctrl.RenderWidth : Width;
            Height += ctrl.RenderHeight;
        }

        /// <summary>
        /// Calculates intermediate size if Orientation is horizontal and Expansion is Expand
        /// </summary>
        /// <param name="ctrl">The control used to calculate the intermediate size.</param>
        private void ResetHorizontalExpand(IControl ctrl)
        {
            if (ctrl.IsHidden)
                return;
            Height = (ctrl.RenderHeight > Height) ? ctrl.RenderHeight : Height;
            UsedMajorDirection += ctrl.RenderWidth;
            Width = (UsedMajorDirection > Width) ? UsedMajorDirection : Width;
        }

        /// <summary>
        /// Calculates intermediate size if Orientation is vertical and Expansion is Expand
        /// </summary>
        /// <param name="ctrl">The control used to calculate the intermediate size.</param>
        private void ResetVerticalExpand(IControl ctrl)
        {
            if (ctrl.IsHidden)
                return;
            Width = (ctrl.RenderWidth > Width) ? ctrl.RenderWidth : Width;
            UsedMajorDirection += ctrl.RenderHeight;
            Height = (UsedMajorDirection > Height) ? UsedMajorDirection : Height;
        }

    }
}
