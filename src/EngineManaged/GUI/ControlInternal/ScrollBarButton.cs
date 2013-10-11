using Flood.GUI.Controls;

namespace Flood.GUI.ControlInternal
{
    /// <summary>
    /// Scrollbar button.
    /// </summary>
    public class ScrollBarButton : Button
    {
        private Pos m_Direction;

        /// <summary>
        /// Initializes a new instance of the <see cref="ScrollBarButton"/> class.
        /// </summary>
        /// <param name="parent">Parent control.</param>
        public ScrollBarButton(Control parent)
            : base(parent)
        {
            SetDirectionUp();
        }

        public virtual void SetDirectionUp()
        {
            m_Direction = Pos.Top;
        }

        public virtual void SetDirectionDown()
        {
            m_Direction = Pos.Bottom;
        }

        public virtual void SetDirectionLeft()
        {
            m_Direction = Pos.Left;
        }

        public virtual void SetDirectionRight()
        {
            m_Direction = Pos.Right;
        }

        /// <summary>
        /// Renders the control using specified skin.
        /// </summary>
        /// <param name="skin">Skin to use.</param>
        protected override void Render(Skins.Skin skin)
        {
            skin.DrawScrollButton(this, m_Direction, IsDepressed, IsHovered, IsDisabled);
        }
    }
}
