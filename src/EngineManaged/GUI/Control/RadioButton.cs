﻿using System;

namespace Gwen.Control
{
    /// <summary>
    /// Radio button.
    /// </summary>
    public class RadioButton : CheckBox
    {
        /// <summary>
        /// Determines whether unchecking is allowed.
        /// </summary>
        protected override bool AllowUncheck
        {
            get { return false; }
        }

        /// <summary>
        /// Initializes a new instance of the <see cref="RadioButton"/> class.
        /// </summary>
        /// <param name="parent">Parent control.</param>
        public RadioButton(Base parent)
            : base(parent)
        {
            SetSize(15, 15);
            MouseInputEnabled = true;
            IsTabable = false;
        }

        /// <summary>
        /// Renders the control using specified skin.
        /// </summary>
        /// <param name="skin">Skin to use.</param>
        protected override void Render(Skin.Base skin)
        {
            skin.DrawRadioButton(this, IsChecked, IsDepressed);
        }
    }
}
