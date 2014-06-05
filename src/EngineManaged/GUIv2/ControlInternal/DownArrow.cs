using Flood.GUIv2.Controls;
using Flood.GUIv2.Panels.Layout;

namespace Flood.GUIv2.ControlInternal
{
    /// <summary>
    /// ComboBox arrow.
    /// </summary>
    public class DownArrow : Control
    {
        private readonly ComboBox m_ComboBox;

        /// <summary>
        /// Initializes a new instance of the <see cref="DownArrow"/> class.
        /// </summary>
        /// <param name="comboBox">Parent control.</param>
        public DownArrow(ComboBox comboBox)
        {
            MouseInputEnabled = false;
            SetSize(15, 15);

            m_ComboBox = comboBox;
        }

        /// <summary>
        /// Renders the control using specified skin.
        /// </summary>
        /// <param name="skin">Skin to use.</param>
        public override void Render(Skins.Skin skin)
        {
            skin.DrawComboBoxArrow(this, m_ComboBox.IsHovered, m_ComboBox.IsDepressed, m_ComboBox.IsOpen, m_ComboBox.IsDisabled);
        }
    }
}
