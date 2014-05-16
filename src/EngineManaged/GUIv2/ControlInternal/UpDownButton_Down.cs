using Flood.GUIv2.Controls;

namespace Flood.GUIv2.ControlInternal
{
    /// <summary>
    /// Numeric down arrow.
    /// </summary>
    internal class UpDownButton_Down : Button
    {
        /// <summary>
        /// Initializes a new instance of the <see cref="UpDownButton_Down"/> class.
        /// </summary>
        /// <param name="parent">Parent control.</param>
        public UpDownButton_Down(Control parent)
            : base(parent)
        {
            SetSize(7, 7);
        }

        /// <summary>
        /// Renders the control using specified skin.
        /// </summary>
        /// <param name="skin">Skin to use.</param>
        public override void Render(Skins.Skin skin)
        {
            skin.DrawNumericUpDownButton(this, IsDepressed, false);
        }
    }
}
