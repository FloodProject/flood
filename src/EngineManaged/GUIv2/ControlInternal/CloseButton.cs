using Flood.GUIv2.Controls;

namespace Flood.GUIv2.ControlInternal
{
    /// <summary>
    /// Window close button.
    /// </summary>
    public class CloseButton : Button
    {
        private readonly WindowControl m_Window;

        /// <summary>
        /// Initializes a new instance of the <see cref="CloseButton"/> class.
        /// </summary>
        /// <param name="parent">Parent control.</param>
        /// <param name="owner">Window that owns this button.</param>
        public CloseButton(Control parent, WindowControl owner)
        {
            m_Window = owner;
        }

        /// <summary>
        /// Renders the control using specified skin.
        /// </summary>
        /// <param name="skin">Skin to use.</param>
        //public override void Render(Skins.Skin skin)
        //{
        //    skin.DrawWindowCloseButton(this, IsDepressed && IsHovered, IsHovered && ShouldDrawHover, !m_Window.IsOnTop);
        //}
    }
}
