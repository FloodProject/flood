using System;
using Flood.GUIv2.Controls;
using Flood.GUIv2.Panels.Layout;

namespace Flood.GUIv2.ControlInternal
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
        {
            throw new NotImplementedException();

            Height = 1;
        }

        public override bool InformFirstDirection(BoxOrientation direction, int size, int availableOtherDir)
        {
            throw new NotImplementedException();
        }

        /// <summary>
        /// Renders the control using specified skin.
        /// </summary>
        /// <param name="skin">Skin to use.</param>
        public override void Render(Skins.Skin skin)
        {
            skin.DrawMenuDivider(this);
        }
    }
}
