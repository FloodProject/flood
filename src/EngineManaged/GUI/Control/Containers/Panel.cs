using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Flood.GUI.Controls.Containers
{
    public class Panel : Control
    {
        /// <summary>
        /// Gets or sets a value indicating whether this panel needs layout 
        /// in case a child has been redimensioned.
        /// </summary>
        public bool NeedsLayout { get; protected set; }

        /// <summary>
        /// Gets a value indicating whether parent is a <see cref="Canvas"/> or another panel.
        /// </summary>
        protected bool ParentIsCanvas { get; private set; }

        /// <summary>
        /// Informs this panel regarding the size available in the minor direction 
        /// of the containing panel so it can call its sizer to redimension itself.
        /// </summary>
        /// <param name="direction">The minor direction.</param>
        /// <param name="size">The size available in the minor panel direction.</param>
        /// <param name="availableOtherDir">The available size in the major panel dir.</param>
        /// <returns>Whether any size reduction was achieved.</returns>
        public override bool InformFirstDirection(BoxOrientation direction, int size, int availableOtherDir)
        {
            Debug.Assert(Sizer != null);
            bool didUse = base.InformFirstDirection(direction, size, availableOtherDir) ||
                Sizer.InformFirstDirection(direction, size, availableOtherDir);
            if (didUse)
                BestSize = Sizer.CalcMin();
            return didUse;
        }

        /// <summary>
        /// Gets the associated sizer.
        /// </summary>
        /// <remarks>
        /// The sizer is set by the sizer constructor when it is passed a reference to this.
        /// The coupling must be done that way.
        /// </remarks>
        public Sizer Sizer { get; internal set; }

        /// <summary>
        /// Initializes a base instance of the <see cref="Panel"/> class. 
        /// This is a top level panel, inheriting from a canvas and should be 
        /// the root for all child panels in this canvas.
        /// </summary>
        /// <param name="parent">The parent canvas.</param>
        /// <param name="expansion">The type of <see cref="ExpansionFlags"/> of this panel.</param>
        public Panel(Canvas parent, ExpansionFlags expansion = ExpansionFlags.Expand) 
            : base(parent)
        {
            ParentIsCanvas = true;
            Expansion = expansion;
        }

        /// <summary>
        /// Initializes a new instance of the <see cref="Panel"/> class.
        /// </summary>
        /// <param name="parent">The parent panel.</param>
        /// <param name="expansion">The type of <see cref="ExpansionFlags"/> of this panel.</param>
        public Panel(Panel parent, ExpansionFlags expansion = ExpansionFlags.Expand) 
            : base(parent)
        {
            ParentIsCanvas = false;
            Expansion = expansion;
        }

        /// <summary>
        /// Invalidates the control.
        /// </summary>
        /// <remarks>
        /// Sets the needs layout flag to true.
        /// </remarks>
        public override void Invalidate()
        {
            base.Invalidate();
            if(Parent != null)
                Parent.Invalidate();
            NeedsLayout = true;
        }

        /// <summary>
        /// Lays out this panel using the associated sizer.
        /// </summary>
        public virtual void Layout()
        {
            if (Sizer == null)
                return;
            Sizer.RecalcSizes();
        }
    }
}
