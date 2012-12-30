﻿using System;
using System.Drawing;
using Gwen.Control;

namespace Gwen.ControlInternal
{
    /// <summary>
    /// Property button.
    /// </summary>
    public class ColorButton : Button
    {
        private Color m_Color;

        /// <summary>
        /// Current color value.
        /// </summary>
        public Color Color { get { return m_Color; } set { m_Color = value; } }

        /// <summary>
        /// Initializes a new instance of the <see cref="ColorButton"/> class.
        /// </summary>
        /// <param name="parent">Parent control.</param>
        public ColorButton(Base parent) : base(parent)
        {
            m_Color = Color.Black;
            Text = String.Empty;
        }

        /// <summary>
        /// Renders the control using specified skin.
        /// </summary>
        /// <param name="skin">Skin to use.</param>
        protected override void Render(Skin.Base skin)
        {
            skin.Renderer.DrawColor = m_Color;
            skin.Renderer.DrawFilledRect(RenderBounds);
        }
    }
}
