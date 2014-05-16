using Flood.GUIv2.ControlInternal;
using Flood.GUIv2.Input;
using System;
using Flood.GUIv2.Panels;
using Flood.GUIv2.Panels.Layout;

namespace Flood.GUIv2.Controls
{
    /// <summary>
    /// Button control.
    /// </summary>
    public class Button : CompositeControl
    {
        private bool m_Depressed;
        private bool m_Toggle;
        private bool m_ToggleStatus;
        private bool m_CenterImage;
        private ImagePanel m_Image;


        private readonly Text m_Text;

        /// <summary>
        /// Text alignment.
        /// </summary>
        public AlignmentFlags TextAlignment { get { return m_Text.Alignment; } set { m_Text.Alignment = value; Invalidate(); } }

        /// <summary>
        /// Text.
        /// </summary>
        public String Text { get { return m_Text.String; } set { SetText(value); } }

        /// <summary>
        /// Font.
        /// </summary>
        public Font Font
        {
            get { return m_Text.Font; }
            set
            {
                m_Text.Font = value;
                SizeToContents();
            }
        }

        public override void OnCanvasChanged(Canvas canvas)
        {
            base.OnCanvasChanged(canvas);
            Layout(Skin);
        }

        /// <summary>
        /// Text color.
        /// </summary>
        public Color TextColor { get { return m_Text.TextColor; } set { m_Text.TextColor = value; } }

        /// <summary>
        /// Override text color (used by tooltips).
        /// </summary>
        public Color TextColorOverride { get { return m_Text.TextColorOverride; } set { m_Text.TextColorOverride = value; } }

        /// <summary>
        /// Text override - used to display different string.
        /// </summary>
        public String TextOverride { get { return m_Text.TextOverride; } set { m_Text.TextOverride = value; } }

        /// <summary>
        /// Width of the text (in pixels).
        /// </summary>
        public int TextWidth { get { return m_Text.Width; } }

        /// <summary>
        /// Height of the text (in pixels).
        /// </summary>
        public int TextHeight { get { return m_Text.Height; } }

        public int TextX { get { return m_Text.X; } }
        public int TextY { get { return m_Text.Y; } }

        /// <summary>
        /// Text length (in characters).
        /// </summary>
        public int TextLength { get { return m_Text.Length; } }
        public int TextRight { get { return m_Text.Right; } }
        public virtual void MakeColorNormal() { TextColor = Skin.Colors.Label.Default; }
        public virtual void MakeColorBright() { TextColor = Skin.Colors.Label.Bright; }
        public virtual void MakeColorDark() { TextColor = Skin.Colors.Label.Dark; }
        public virtual void MakeColorHighlight() { TextColor = Skin.Colors.Label.Highlight; }

        /// <summary>
        /// Text margin.
        /// </summary>
        public Margin TextMargin { get { return m_Text.Margin; } set { m_Text.Margin = value; Invalidate(); } }


        /// <summary>
        /// Invoked when the button is released.
        /// </summary>
        public event Control.GwenEventHandler Clicked;

        /// <summary>
        /// Invoked when the button is pressed.
        /// </summary>
        public event Control.GwenEventHandler Pressed;

        /// <summary>
        /// Invoked when the button is released.
        /// </summary>
        public event Control.GwenEventHandler Released;

        /// <summary>
        /// Invoked when the button's toggle state has changed.
        /// </summary>
        public event Control.GwenEventHandler Toggled;

        /// <summary>
        /// Invoked when the button's toggle state has changed to On.
        /// </summary>
        public event Control.GwenEventHandler ToggledOn;

        /// <summary>
        /// Invoked when the button's toggle state has changed to Off.
        /// </summary>
        public event Control.GwenEventHandler ToggledOff;

        /// <summary>
        /// Invoked when the button has been double clicked.
        /// </summary>
        public event Control.GwenEventHandler DoubleClickedLeft;

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
        public Button()
            : base(new DockPanel())
        {
            SetSize(100, 20);
            MouseInputEnabled = true;
            TextAlignment = AlignmentFlags.Center;
            TextMargin = Margin.Three;
        }

        /// <summary>
        /// Sets the label text.
        /// </summary>
        /// <param name="str">Text to set.</param>
        /// <param name="doEvents">Determines whether to invoke "text changed" event.</param>
        public virtual void SetText(String str, bool doEvents = true)
        {
            if (Text == str)
                return;

            m_Text.String = str;
            SizeToContents();
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
        public virtual void Press(IControl control = null)
        {
            OnClicked();
        }

        /// <summary>
        /// Renders the control using specified skin.
        /// </summary>
        /// <param name="skin">Skin to use.</param>
        public override void Render(Skins.Skin skin)
        {
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
        public override void OnMouseClickedLeft(int x, int y, bool down)
        {
            //base.OnMouseClickedLeft(x, y, down);
            if (down)
            {
                IsDepressed = true;
                InputHandler.MouseFocus = this;
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
                InputHandler.MouseFocus = null;
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
        public void SetImage(String textureName, bool center = false)
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

            //m_Image.Texture.Image = new Image();
            throw new NotImplementedException();

            m_Image.SizeToContents();
            m_Image.SetPosition(2, 2);
            m_CenterImage = center;

            TextMargin = new Margin(m_Image.Right + 2, TextMargin.Top, TextMargin.Right, TextMargin.Bottom);
        }

        /// <summary>
        /// Sets the button's image.
        /// </summary>
        /// <param name="image">Image bitmap.</param>
        /// <param name="center">Determines whether the image should be centered.</param>
        public virtual void SetImage(ResourceHandle<Image> imageHandle, bool center = false)
        {
            throw new NotImplementedException();
            
            //if (m_Image == null)
            //{
            //    m_Image = new ImagePanel(this);
            //}

            //m_Image.SetImage(imageHandle);

            //m_Image.SizeToContents( );
            //m_Image.SetPosition(Math.Max(Padding.Left, 2), 2);
            //m_CenterImage = center;

            //TextMargin = new Padding(m_Image.Right + 2, TextMargin.Top, TextMargin.Right, TextMargin.Bottom);
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
        public override bool OnKeySpace(bool down)
        {
            if (down)
                OnClicked();
            return true;
        }

        /// <summary>
        /// Default accelerator handler.
        /// </summary>
        //protected override void OnAccelerator()
        //{
        //    OnClicked();
        //}

        /// <summary>
        /// Lays out the control's interior according to alignment, padding, dock etc.
        /// </summary>
        /// <param name="skin">Skin to use.</param>
        public override void Layout(Skins.Skin skin)
        {
            throw new NotImplementedException();
            //base.Layout(skin);
            //if (m_Image != null)
            //{
            //    Align.CenterVertically(m_Image);

            //    if (m_CenterImage)
            //        Align.CenterHorizontally(m_Image);
            //}
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
        public override void OnMouseDoubleClickedLeft(int x, int y)
        {
            throw new NotImplementedException();
            OnMouseClickedLeft(x, y, true);
            if (DoubleClickedLeft != null)
                DoubleClickedLeft.Invoke(this);
        }
    }
}
