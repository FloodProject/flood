using System;
using EngineManaged.GUI.Controls;

namespace EngineManaged.GUI.ControlInternal
{
    /// <summary>
    /// Divider menu item.
    /// </summary>
    public class MenuDivider : Control
    {
        /// <summary>
        /// Initializes a new instance of the <see cref="MenuDivider"/> class.
        /// </summary>
        /// <param name="parent">Parent control.</param>
        public MenuDivider(Control parent)
            : base(parent)
        {
            Height = 1;
        }

        /// <summary>
        /// Renders the control using specified skin.
        /// </summary>
        /// <param name="skin">Skin to use.</param>
        protected override void Render(Skin.Base skin)
        {
            skin.DrawMenuDivider(this);
        }
    }
}
