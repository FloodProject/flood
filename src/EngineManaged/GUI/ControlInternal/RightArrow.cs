using System;
using EngineManaged.GUI.Controls;

namespace EngineManaged.GUI.ControlInternal
{
    /// <summary>
    /// Submenu indicator.
    /// </summary>
    public class RightArrow : Control
    {
        /// <summary>
        /// Initializes a new instance of the <see cref="RightArrow"/> class.
        /// </summary>
        /// <param name="parent">Parent control.</param>
        public RightArrow(Control parent)
            : base(parent)
        {
            MouseInputEnabled = false;
        }

        /// <summary>
        /// Renders the control using specified skin.
        /// </summary>
        /// <param name="skin">Skin to use.</param>
        protected override void Render(Skin.Base skin)
        {
            skin.DrawMenuRightArrow(this);
        }
    }
}
