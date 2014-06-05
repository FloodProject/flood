using Flood.GUIv2.Controls;
using Flood.GUIv2.Panels.Layout;

namespace Flood.GUIv2.ControlInternal
{
    /// <summary>
    /// Color square.
    /// </summary>
    public class ColorDisplay : Control
    {
        private Color m_Color;
        //private bool m_DrawCheckers;

        /// <summary>
        /// Initializes a new instance of the <see cref="ColorDisplay"/> class.
        /// </summary>
        /// <param name="parent">Parent control.</param>
        public ColorDisplay()
        {
            SetSize(32, 32);
            m_Color = new Color(255, 0, 0, 255);
            //m_DrawCheckers = true;
        }

        /// <summary>
        /// Renders the control using specified skin.
        /// </summary>
        /// <param name="skin">Skin to use.</param>
        public override void Render(Skins.Skin skin)
        {
            skin.DrawColorDisplay(this, m_Color);
        }

        /// <summary>
        /// Current color.
        /// </summary>
        public Color Color { get { return m_Color; } set { m_Color = value; } }
        //public bool DrawCheckers { get { return m_DrawCheckers; } set { m_DrawCheckers = value; } }
        public byte R { get { return m_Color.R; } set { m_Color = new Color(value, m_Color.G, m_Color.B, m_Color.A); } }
        public byte G { get { return m_Color.G; } set { m_Color = new Color(m_Color.R, value, m_Color.B, m_Color.A); } }
        public byte B { get { return m_Color.B; } set { m_Color = new Color(m_Color.R, m_Color.G, value, m_Color.A); } }
        public byte A { get { return m_Color.A; } set { m_Color = new Color( m_Color.R, m_Color.G, m_Color.B, value); } }
    }
}
