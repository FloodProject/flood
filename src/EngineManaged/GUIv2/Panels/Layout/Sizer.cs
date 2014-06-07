using System;
using System.Collections.Generic;

namespace Flood.GUIv2.Panels.Layout
{

    public class NullParentException: GuiException
    {
    }

    public class GuiException: ApplicationException
    {
    }

    /// <summary>
    /// Margin flags. Indicates which margins are set.
    /// </summary>
    [Flags]
    public enum MarginFlags:byte
    {
        NotSet = 0,
        Top = 1,
        Bottom = 2,
        Left = 4,
        Right = 8,
        All = Top | Bottom | Left | Right,
    }

    /// <summary>
    /// Padding flags. Indicates which padding directions are set.
    /// </summary>
    [Flags]
    public enum PaddingFlags:byte
    {
        NotSet = 0,
        Top = 1,
        Bottom = 2,
        Left = 4,
        Right = 8,
        All = Top | Bottom | Left | Right,
    }

    /// <summary>
    /// Expansion flags. Indicates how control tries to occupy space.
    /// </summary>
    /// <remarks>
    ///  NotSet - expansion type undefined
    ///  Expand - try to occupy all space available
    ///  Shaped - set to occupy current width and height
    ///  Fit - redimension to fit content
    /// </remarks>
    [Flags]
    public enum ExpansionFlags
    {
        Shaped = 0,
        ExpandHorizontal = 1 << 0,
        ExpandVertical = 1 << 1,
        FillVertical = 1 << 2,
        FillHorizontal = 1 << 3,
        Fit = 1 << 4,
        Expand = ExpandHorizontal | ExpandVertical,
        Fill = FillHorizontal | FillVertical,
        FitExpand = Expand | Fit,
        FitExpandHorizontal = ExpandHorizontal | Fit,
        FitExpandVertical = ExpandVertical | Fit,
    }

    //todo: split alignment in vertical and horizontal enums
    /// <summary>
    /// Alignment flags. Type of alignment.
    /// </summary>
    [Flags]
    public enum AlignmentFlags
    {
        Center = 1 << 0,
        Left = 1 << 1,
        Right = 1 << 2,
        Top = 1 << 3,
        Bottom = 1 << 4,
        CenterVertical = 1 << 5,
        CenterHorizontal = 1 << 6,
        NotSet = 1 << 7
    }

    /// <summary>
    /// Base sizer class.
    /// A sizer manages the layout of an associated 
    /// <see cref="Panel" />.
    /// </summary>
    public abstract class Sizer
    {
        /// <summary>
        /// Gets or sets the size of the associated <see cref="Panel"/>.
        /// </summary>
        public Vector2i Size { 
            get
            {
                if (Panel == null)
                    return default(Vector2i);
                return new Vector2i(Panel.Width, Panel.Height);
            }
            set
            {
                if (Panel == null)
                    return;
                Panel.Width = value.X;
                Panel.Height = value.Y;
            }
        }


        /// <summary>
        /// Gets or sets the position of the associated <see cref="Panel"/>.
        /// </summary>
        public Vector2i Position
        {
            get
            {
                if (Panel == null)
                    return default(Vector2i);
                return new Vector2i(Panel.X, Panel.Y);
            }
            set
            {
                if (Panel == null)
                    return;
                Panel.X = value.X;
                Panel.Y = value.Y;
            }
        }

        /// <summary>
        /// <see cref="WeakReference"/> to the <see cref="Panel"/> associated to this <see cref="Sizer"/>.
        /// </summary>
        protected WeakReference m_panel;

        /// <summary>
        /// Gets or sets the associated <see cref="Panel"/>.
        /// </summary>
        public Panel Panel
        {
            get { return (m_panel != null && m_panel.IsAlive)? m_panel.Target as Panel : null; } 
            set
            {
                m_panel = new WeakReference(value);
                Panel.Sizer = this;
            }
        }

        /// <summary>
        /// Gets the children of the associated <see cref="Panel"/>.
        /// </summary>
        public List<IControl> Children 
        { 
            get
            {
                if (Panel == null)
                    return null;
                return Panel.Children;
            } 
        }

        /// <summary>
        /// Initializes a new instance of the <see cref="Sizer"/> class.
        /// </summary>
        /// <param name="panel">The associated <see cref="Panel"/>.</param>
        /// <remarks>
        /// The sizer creates the association between the panel and itself. The panel
        /// shouldn't do anything, all that is required is for it to be passed to the sizer
        /// in its constructor.
        /// </remarks>
        protected Sizer(Panel panel)
        {
            Panel = panel;
        }

        #region Abstract methods

        /// <summary>
        /// Main layout function, repositions and rescales panel's childen according to type of panel.
        /// </summary>
        public abstract void RecalcSizes();

        /// <summary>
        /// Calculates the minimum size of the associated panel.
        /// </summary>
        /// <returns> The calculated min size.</returns>
        public abstract Vector2i CalcMin(); //todo: remove this when finished replacing panels architecture

        #endregion

        public void PositionAndAlign(Vector2i boxPosition, Vector2i boxSize, IControl control)
        {
            var size = control.LayoutMinSize - control.MarginSizes;
            if (boxSize.GE(control.LayoutBestSize))
                size = control.LayoutBestSize - control.MarginSizes;

            if (control.IsExpandHorizontal || control.IsFillHorizontal)
                size.X = Math.Min(boxSize.X, control.LayoutMaxSize.X) - control.HorizontalMargins;
            if (control.IsExpandVertical || control.IsFillVertical)
                size.Y = Math.Min(boxSize.Y, control.LayoutMaxSize.Y) - control.VerticalMargins;

            if(size.X <= 0 || size.Y <= 0)
                throw new Exception("Controls must have positive dimensions.");

            var position = new Vector2i(boxPosition.X, boxPosition.Y);

            if ((control.Alignment & AlignmentFlags.Right) != 0)
                position.X += boxSize.X - size.X - control.Margin.Right;
            if ((control.Alignment & AlignmentFlags.Left) != 0)
                position.X += control.Margin.Left;
            if ((control.Alignment & AlignmentFlags.Bottom) != 0)
                position.Y += boxSize.Y - size.Y - control.Margin.Bottom;
            if ((control.Alignment & AlignmentFlags.Top) != 0)
                position.Y += control.Margin.Top;
            if ((control.Alignment & (AlignmentFlags.CenterHorizontal | AlignmentFlags.Center)) != 0)
                position.X += (boxSize.X - size.X)/2; 
            if ((control.Alignment & (AlignmentFlags.CenterVertical | AlignmentFlags.Center)) != 0)
                position.Y += (boxSize.Y - size.Y)/2; 

            control.SetDimension(position, size);
        }


    }
}
