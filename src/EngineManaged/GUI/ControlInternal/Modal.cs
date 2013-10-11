﻿using Flood.GUI.Controls;

namespace Flood.GUI.ControlInternal
{
    /// <summary>
    /// Modal control for windows.
    /// </summary>
    public class Modal : Control
    {
        /// <summary>
        /// Initializes a new instance of the <see cref="Modal"/> class.
        /// </summary>
        /// <param name="parent">Parent control.</param>
        public Modal(Control parent)
            : base(parent)
        {
            KeyboardInputEnabled = true;
            MouseInputEnabled = true;
            ShouldDrawBackground = true;
            SetBounds(0, 0, GetCanvas().Width, GetCanvas().Height);
        }
        
        /// <summary>
        /// Lays out the control's interior according to alignment, padding, dock etc.
        /// </summary>
        /// <param name="skin">Skin to use.</param>
        protected override void Layout(Skins.Skin skin)
        {
            SetBounds(0, 0, GetCanvas().Width, GetCanvas().Height);
        }

        /// <summary>
        /// Renders the control using specified skin.
        /// </summary>
        /// <param name="skin">Skin to use.</param>
        protected override void Render(Skins.Skin skin)
        {
            skin.DrawModalControl(this);
        }
    }
}
