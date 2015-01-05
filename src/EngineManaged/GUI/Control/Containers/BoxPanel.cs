using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Flood.GUI.Controls.Containers
{
    public class BoxPanel : Panel
    {
        /// <summary>
        /// The used space in the major direction
        /// </summary>
        private int UsedMajorDirection = 0;

        private BoxOrientation m_orientation;

        /// <summary>
        /// Gets or sets the box panel orientation.
        /// </summary>
        /// <remarks>
        /// If orientation is altered the pane is laid out again
        /// </remarks>
        public BoxOrientation Orientation
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
        /// <param name="parent">The parent panel.</param>
        /// <param name="orientation">The orientation of the box panel.</param>
        /// <param name="expansion">The type of <see cref="ExpansionFlags"/> of this panel.</param>
        public BoxPanel(Panel parent, BoxOrientation orientation = BoxOrientation.Vertical, ExpansionFlags expansion = ExpansionFlags.Fit)
            : base(parent, expansion)
        {
            Init(orientation);
        }

        /// <summary>
        /// Initializes a new instance of the <see cref="BoxPanel"/> class as a top level panel.
        /// </summary>
        /// <param name="parent">The parent canvas.</param>
        /// <param name="orientation">The orientation of the box panel.</param>
        /// <param name="expansion">The type of <see cref="ExpansionFlags"/> of this panel.</param>
        public BoxPanel(Canvas parent, BoxOrientation orientation = BoxOrientation.Vertical, ExpansionFlags expansion = ExpansionFlags.Fit)
            : base(parent, expansion)
        {
            Init(orientation);
        }

        private void Init(BoxOrientation orientation)
        {
            Orientation = orientation;
            if (Expansion == ExpansionFlags.Fit)
            {
                Width = 0;
                Height = 0;
            }
            new BoxSizer(this);
        }

        /// <summary>
        /// Adds a child control to the panel.
        /// </summary>
        /// <param name="control">The child control.</param>
        public override void AddChild(Control control)
        {
            base.AddChild(control);
            AddControl(control);
        }

        /// <summary>
        /// Adhusts panel's dimensions based on control added and type of panel expansion.
        /// </summary>
        /// <param name="control">The control added.</param>
        private void AddControl(Control control)
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
        private void AddControlFit(Control control)
        {
            if (control.IsHidden || !control.IsRenderable)
                return;

            if (Orientation == BoxOrientation.Vertical)
            {
                Width = (control.Width > Width) ? control.Width : Width;
                Height += control.Height;
            }
            else
            {
                Height = (control.Height > Height) ? control.Height : Height;
                Width += control.Width;
            }
            VerifyBounds();
            Invalidate();
        }

        /// <summary>
        /// Adjusts panel's dimensions when a new control is added for <see cref="ExpansionFlags"/> type Expand.
        /// </summary>
        /// <param name="control">The control added.</param>
        private void AddControlExpand(Control control)
        {
            if (control.IsHidden || !control.IsRenderable)
                return;

            if (Orientation == BoxOrientation.Vertical)
            {
                Width = (control.Width > Width) ? control.Width : Width;
                UsedMajorDirection += control.Height;
                Height = (UsedMajorDirection > Height) ? UsedMajorDirection : Height;
            }
            else
            {
                Height = (control.Height > Height) ? control.Height : Height;
                UsedMajorDirection += control.Width;
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
        public override void RemoveChild(Control child, bool dispose)
        {
            base.RemoveChild(child, dispose);
            if(!dispose)
                Trim(child);
            Layout();
        }

        /// <summary>
        /// Resizes panel when  a control is removed and the expansion type is Fit.
        /// </summary>
        /// <param name="control">The removed control.</param>
        private void Trim(Control control)
        {
            if (control.IsHidden || !control.IsRenderable || Expansion != ExpansionFlags.Fit)
                return;

            if (Children.Count == 0)
            {
                Width = 0;
                Height = 0;
            }
            else
            {
                if (Orientation == BoxOrientation.Vertical)
                {
                    Width = (control.Width == Width)? 
                        Children.OrderByDescending(x => x.Width).FirstOrDefault().Width : Width;
                    Height -= control.Height;
                }
                else
                {
                    Height = (control.Height == Height) ? 
                        Children.OrderByDescending(x => x.Height).FirstOrDefault().Height : Height;
                    Width -= control.Width;
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
            Layout();
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
            Layout();
        }

        /// <summary>
        /// Verifies if max or min bounds have been crossed and adjusts accordingly.
        /// </summary>
        private void VerifyBounds()
        {
            //check if bounds are set and if so if the values exceed them and adjust accordingly
            if(MaximumSize.X != SizeNotSet && Width > MaximumSize.X)
                Width = MaximumSize.X;
            else if (MinimumSize.X != SizeNotSet && Width < MinimumSize.X)
                Width = MinimumSize.X;

            if (MaximumSize.Y != SizeNotSet && Height > MaximumSize.Y)
                Height = MaximumSize.Y;
            else if (MinimumSize.Y != SizeNotSet && Height < MinimumSize.Y)
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
            if (Orientation == BoxOrientation.Horizontal)
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
        private delegate void ResetOrientationFunction(Control control);

        /// <summary>
        /// Calculates intermediate size if Orientation is horizontal and Expansion is Fit
        /// </summary>
        /// <param name="ctrl">The control used to calculate the intermediate size.</param>
        private void ResetHorizontalFit(Control ctrl)
        {
            if (ctrl.IsHidden || !ctrl.IsRenderable)
                return;
            Height = (ctrl.Height > Height) ? ctrl.Height : Height;
            Width += ctrl.Width;
        }

        /// <summary>
        /// Calculates intermediate size if Orientation is vertical and Expansion is Fit
        /// </summary>
        /// <param name="ctrl">The control used to calculate the intermediate size.</param>

        private void ResetVerticalFit(Control ctrl)
        {
            if (ctrl.IsHidden || !ctrl.IsRenderable)
                return;
            Width = (ctrl.Width > Width) ? ctrl.Width : Width;
            Height += ctrl.Height;
        }

        /// <summary>
        /// Calculates intermediate size if Orientation is horizontal and Expansion is Expand
        /// </summary>
        /// <param name="ctrl">The control used to calculate the intermediate size.</param>
        private void ResetHorizontalExpand(Control ctrl)
        {
            if (ctrl.IsHidden || !ctrl.IsRenderable)
                return;
            Height = (ctrl.Height > Height) ? ctrl.Height : Height;
            UsedMajorDirection += ctrl.Width;
            Width = (UsedMajorDirection > Width) ? UsedMajorDirection : Width;
        }

        /// <summary>
        /// Calculates intermediate size if Orientation is vertical and Expansion is Expand
        /// </summary>
        /// <param name="ctrl">The control used to calculate the intermediate size.</param>
        private void ResetVerticalExpand(Control ctrl)
        {
            if (ctrl.IsHidden || !ctrl.IsRenderable)
                return;
            Width = (ctrl.Width > Width) ? ctrl.Width : Width;
            UsedMajorDirection += ctrl.Height;
            Height = (UsedMajorDirection > Height) ? UsedMajorDirection : Height;
        }

    }
}
