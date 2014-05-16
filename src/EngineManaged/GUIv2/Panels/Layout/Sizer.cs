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
        NotSet = 1,
        Expand = 2,
        Shaped = 4,
        Fit = 8
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
    /// A sizer manages the layout of an associated <see cref="Panel"/>.
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
        public abstract Vector2i CalcMin();

        /// <summary>
        /// Informs this sizer regarding the size available in the minor direction 
        /// of the containing panel so it can redimension the associated panel.
        /// </summary>
        /// <param name="direction">The minor direction.</param>
        /// <param name="size">The size available in the minor panel direction.</param>
        /// <param name="availableOtherDir">The available size in the major panel dir.</param>
        /// <returns>Whether any size reduction was achieved.</returns>
        public abstract bool InformFirstDirection(BoxOrientation direction, int size, int availableOtherDir);

        #endregion
    }
}
