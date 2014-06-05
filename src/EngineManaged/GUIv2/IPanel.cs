using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Flood.GUIv2
{
    public interface IPanel
    {
        void LayoutSizes();

        void RecursiveLayoutSizes(Skins.Skin skin);

        void LayoutAssign();

        void RecursiveLayoutAssign();

        /// <summary>
        /// Gets or sets a value indicating whether this control needs layout
        /// </summary>
        bool NeedsLayout { get; set; }

        /// <summary>
        /// Invalidates the control.
        /// </summary>
        /// <remarks>
        /// Causes layout, repaint, invalidates cached texture.
        /// </remarks>
        void Invalidate();

        /// <summary>
        /// Invalidates the control's children (relayout/repaint).
        /// </summary>
        /// <param name="recursive">Determines whether the operation should be carried recursively.</param>
        void InvalidateChildren(bool recursive = false);

    }
}
