﻿using System;
using Gwen.Input;

namespace Gwen.Control
{
    /// <summary>
    /// Button control.
    /// </summary>
    public class Button : Label
    {
        private bool m_Depressed;
        private bool m_Toggle;
        private bool m_ToggleStatus;
        private bool m_CenterImage;
        private ImagePanel m_Image;

        /// <summary>
        /// Invoked when the button is released.
        /// </summary>
        public event GwenEventHandler Clicked;

        /// <summary>
        /// Invoked when the button is pressed.
        /// </summary>
        public event GwenEventHandler Pressed;

        /// <summary>
        /// Invoked when the button is released.
        /// </summary>
        public event GwenEventHandler Released;

        /// <summary>
        /// Invoked when the button's toggle state has changed.
        /// </summary>
        public event GwenEventHandler Toggled;

        /// <summary>
        /// Invoked when the button's toggle state has changed to On.
        /// </summary>
        public event GwenEventHandler ToggledOn;

        /// <summary>
        /// Invoked when the button's toggle state has changed to Off.
        /// </summary>
        public event GwenEventHandler ToggledOff;

        /// <summary>
        /// Invoked when the button has been double clicked.
        /// </summary>
        public event GwenEventHandler DoubleClickedLeft;

        /// <summary>
        /// Indicates whether the button is depressed.
        /// </summary>
        public bool IsDepressed
        {
            get { return m_Depressed; }
            set
            {
                if (m_Depressed == value) 
                    return;
                m_Depressed = value; 
                Redraw();
            }
        }

        /// <summary>
        /// Indicates whether the button is toggleable.
        /// </summary>
        public bool IsToggle { get { return m_Toggle; } set { m_Toggle = value; } }

        /// <summary>
        /// Determines the button's toggle state.
        /// </summary>
        public bool ToggleState
        {
            get { return m_ToggleStatus; }
            set
            {
                if (!m_Toggle) return;
                if (m_ToggleStatus == value) return;

                m_ToggleStatus = value;

                if (Toggled != null)
                    Toggled.Invoke(this);

                if (m_ToggleStatus)
                {
                    if (ToggledOn != null)
                        ToggledOn.Invoke(this);
                }
                else
                {
                    if (ToggledOff != null)
                        ToggledOff.Invoke(this);
                }

                Redraw();
            }
        }

        /// <summary>
        /// Control constructor.
        /// </summary>
        /// <param name="parent">Parent control.</param>
        public Button(Base parent)
            : base(parent)
        {
            SetSize(100, 20);
            MouseInputEnabled = true;
            Alignment = Pos.Center;
            TextPadding = new Padding(3, 3, 3, 3);
        }

        /// <summary>
        /// Toggles the button.
        /// </summary>
        public virtual void Toggle()
        {
            ToggleState = !ToggleState;
        }

        /// <summary>
        /// "Clicks" the button.
        /// </summary>
        public virtual void Press(Base control = null)
        {
            OnClicked();
        }

        /// <summary>
        /// Renders the control using specified skin.
        /// </summary>
        /// <param name="skin">Skin to use.</param>
        protected override void Render(Skin.Base skin)
        {
            base.Render(skin);

            if (ShouldDrawBackground)
            {
                bool drawDepressed = IsDepressed && IsHovered;
                if (IsToggle)
                    drawDepressed = drawDepressed || ToggleState;

                bool bDrawHovered = IsHovered && ShouldDrawHover;

                skin.DrawButton(this, drawDepressed, bDrawHovered, IsDisabled);
            }
        }

        /// <summary>
        /// Handler invoked on mouse click (left) event.
        /// </summary>
        /// <param name="x">X coordinate.</param>
        /// <param name="y">Y coordinate.</param>
        /// <param name="down">If set to <c>true</c> mouse button is down.</param>
        protected override void OnMouseClickedLeft(int x, int y, bool down)
        {
            //base.OnMouseClickedLeft(x, y, down);
            if (down)
            {
                IsDepressed = true;
                InputHandler.Instance.MouseFocus = this;
                if (Pressed != null)
                    Pressed.Invoke(this);
            }
            else
            {
                if (IsHovered && m_Depressed)
                {
                    OnClicked();
                }

                IsDepressed = false;
                InputHandler.Instance.MouseFocus = null;
                if (Released != null)
                    Released.Invoke(this);
            }

            Redraw();
        }

        /// <summary>
        /// Internal OnPressed implementation.
        /// </summary>
        protected virtual void OnClicked()
        {
            if (IsToggle)
            {
                Toggle();
            }

            if (Clicked != null)
                Clicked.Invoke(this);
        }
        
        /// <summary>
        /// Sets the button's image.
        /// </summary>
        /// <param name="textureName">Texture name. Null to remove.</param>
        /// <param name="center">Determines whether the image should be centered.</param>
        public virtual void SetImage(String textureName, bool center = false)
        {
            if (String.IsNullOrEmpty(textureName))
            {
                if (m_Image != null)
                    m_Image.Dispose();
                m_Image = null;
                return;
            }

            if (m_Image == null)
            {
                m_Image = new ImagePanel(this);
            }

            m_Image.ImageName = textureName;
            m_Image.SizeToContents( );
            m_Image.SetPosition(Math.Max(Padding.Left, 2), 2);
            m_CenterImage = center;

            TextPadding = new Padding(m_Image.Right + 2, TextPadding.Top, TextPadding.Right, TextPadding.Bottom);
        }

        /// <summary>
        /// Sizes to contents.
        /// </summary>
        public override void SizeToContents()
        {
            base.SizeToContents();
            if (m_Image != null)
            {
                int height = m_Image.Height + 4;
                if (Height < height)
                {
                    Height = height;
                }
            }
        }

        /// <summary>
        /// Handler for Space keyboard event.
        /// </summary>
        /// <param name="down">Indicates whether the key was pressed or released.</param>
        /// <returns>
        /// True if handled.
        /// </returns>
        protected override bool OnKeySpace(bool down)
        {
            if (down)
                OnClicked();
            return true;
        }

        /// <summary>
        /// Default accelerator handler.
        /// </summary>
        protected override void OnAccelerator()
        {
            OnClicked();
        }

        /// <summary>
        /// Lays out the control's interior according to alignment, padding, dock etc.
        /// </summary>
        /// <param name="skin">Skin to use.</param>
        protected override void Layout(Skin.Base skin)
        {
            base.Layout(skin);
            if (m_Image != null)
            {
                Align.CenterVertically(m_Image);

                if (m_CenterImage)
                    Align.CenterHorizontally(m_Image);
            }
        }

        /// <summary>
        /// Updates control colors.
        /// </summary>
        public override void UpdateColors()
        {
            if (IsDisabled)
            {
                TextColor = Skin.Colors.Button.Disabled;
                return;
            }

            if (IsDepressed || ToggleState)
            {
                TextColor = Skin.Colors.Button.Down;
                return;
            }

            if (IsHovered)
            {
                TextColor = Skin.Colors.Button.Hover;
                return;
            }

            TextColor = Skin.Colors.Button.Normal;
        }

        /// <summary>
        /// Handler invoked on mouse double click (left) event.
        /// </summary>
        /// <param name="x">X coordinate.</param>
        /// <param name="y">Y coordinate.</param>
        protected override void OnMouseDoubleClickedLeft(int x, int y)
        {
            OnMouseClickedLeft(x, y, true);
            if (DoubleClickedLeft != null)
                DoubleClickedLeft.Invoke(this);
        }
    }
}
