using System;
using System.Drawing;
using System.Linq;
using EngineManaged.GUI.ControlInternal;

namespace EngineManaged.GUI.Control
{
    /// <summary>
    /// Movable window with title bar.
    /// </summary>
    public class WindowControl : ResizableControl
    {
        private readonly Dragger m_TitleBar;
        private readonly Label m_Caption;
        private readonly CloseButton m_CloseButton;
        private bool m_DeleteOnClose;
        private Modal m_Modal;

        /// <summary>
        /// Window caption.
        /// </summary>
        public String Caption { get { return m_Caption.Text; } set { m_Caption.Text = value; } }

        /// <summary>
        /// Determines whether the window has close button.
        /// </summary>
        public bool IsClosable { get { return !m_CloseButton.IsHidden; } set { m_CloseButton.IsHidden = !value; } }

        /// <summary>
        /// Determines whether the control should be disposed on close.
        /// </summary>
        public bool DeleteOnClose { get { return m_DeleteOnClose; } set { m_DeleteOnClose = value; } }

        /// <summary>
        /// Indicates whether the control is hidden.
        /// </summary>
        public override bool IsHidden
        {
            get { return base.IsHidden; }
            set
            {
                if (!value)
                    BringToFront();
                base.IsHidden = value;
            }
        }

        /// <summary>
        /// Initializes a new instance of the <see cref="WindowControl"/> class.
        /// </summary>
        /// <param name="parent">Parent control.</param>
        /// <param name="caption">Window caption.</param>
        /// <param name="modal">Determines whether the window should be modal.</param>
        public WindowControl(Base parent, String caption = "", bool modal = false)
            : base(parent)
        {
            m_TitleBar = new Dragger(this);
            m_TitleBar.Height = 24;
            m_TitleBar.Padding = EngineManaged.GUI.Padding.Zero;
            m_TitleBar.Margin = new Margin(0, 0, 0, 4);
            m_TitleBar.Target = this;
            m_TitleBar.Dock = Pos.Top;

            m_Caption = new Label(m_TitleBar);
            m_Caption.Alignment = Pos.Left | Pos.CenterV;
            m_Caption.Text = caption;
            m_Caption.Dock = Pos.Fill;
            m_Caption.Padding = new Padding(8, 0, 0, 0);
            m_Caption.TextColor = Skin.Colors.Window.TitleInactive;

            m_CloseButton = new CloseButton(m_TitleBar, this);
            //m_CloseButton.Text = String.Empty;
            m_CloseButton.SetSize(24, 24);
            m_CloseButton.Dock = Pos.Right;
            m_CloseButton.Clicked += CloseButtonPressed;
            m_CloseButton.IsTabable = false;
            m_CloseButton.Name = "closeButton";

            //Create a blank content control, dock it to the top - Should this be a ScrollControl?
            m_InnerPanel = new Base(this);
            m_InnerPanel.Dock = Pos.Fill;
            GetResizer(8).Hide();
            BringToFront();
            IsTabable = false;
            Focus();
            MinimumSize = new Point(100, 40);
            ClampMovement = true;
            KeyboardInputEnabled = false;

            if (modal)
                MakeModal();
        }

        protected virtual void CloseButtonPressed(Base control)
        {
            IsHidden = true;

            if (m_Modal != null)
            {
                m_Modal.DelayedDelete();
                m_Modal = null;
            }

            if (m_DeleteOnClose)
            {
                Parent.RemoveChild(this, true);
            }
        }

        /// <summary>
        /// Makes the window modal: covers the whole canvas and gets all input.
        /// </summary>
        /// <param name="dim">Determines whether all the background should be dimmed.</param>
        public void MakeModal(bool dim = false)
        {
            if (m_Modal != null)
                return;

            m_Modal = new Modal(GetCanvas());
            Parent = m_Modal;

            if (dim)
                m_Modal.ShouldDrawBackground = true;
            else
                m_Modal.ShouldDrawBackground = false;
        }

        /// <summary>
        /// Indicates whether the control is on top of its parent's children.
        /// </summary>
        public override bool IsOnTop
        {
            get { return Parent.Children.Where(x => x is WindowControl).Last() == this; }
        }

        /// <summary>
        /// Renders the control using specified skin.
        /// </summary>
        /// <param name="skin">Skin to use.</param>
        protected override void Render(Skin.Base skin)
        {
            bool hasFocus = IsOnTop;

            if (hasFocus)
                m_Caption.TextColor = Skin.Colors.Window.TitleActive;
            else
                m_Caption.TextColor = Skin.Colors.Window.TitleInactive;

            skin.DrawWindow(this, m_TitleBar.Bottom, hasFocus);
        }

        /// <summary>
        /// Renders under the actual control (shadows etc).
        /// </summary>
        /// <param name="skin">Skin to use.</param>
        protected override void RenderUnder(Skin.Base skin)
        {
            base.RenderUnder(skin);
            skin.DrawShadow(this);
        }

        public override void Touch()
        {
            base.Touch();
            BringToFront();
        }

        /// <summary>
        /// Renders the focus overlay.
        /// </summary>
        /// <param name="skin">Skin to use.</param>
        protected override void RenderFocus(Skin.Base skin)
        {
            
        }
    }
}
