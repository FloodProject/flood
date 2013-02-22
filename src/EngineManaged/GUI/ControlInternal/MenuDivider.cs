using System;
using Flood.GUI.Controls;

namespace Flood.GUI.ControlInternal
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
        protected override void Render(Skins.Skin skin)
        {
            skin.DrawMenuDivider(this);
        }
    }
}
