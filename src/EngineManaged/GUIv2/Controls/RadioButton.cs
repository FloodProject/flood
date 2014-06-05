
using System;

namespace Flood.GUIv2.Controls
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
        public RadioButton(Control parent)
            : base(parent)
        {
            //AutoSizeToContents = false;
            SetSize(15, 15);
            MouseInputEnabled = true;
            IsTabable = false;
        }

        /// <summary>
        /// Renders the control using specified skin.
        /// </summary>
        /// <param name="skin">Skin to use.</param>
        //public override void Render(Skins.Skin skin)
        //{
        //    throw new NotImplementedException();

        //    skin.DrawRadioButton(this, IsChecked, IsDepressed);
        //}
    }
}
