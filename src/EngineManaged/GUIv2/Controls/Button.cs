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

        public override AlignmentFlags Alignment
        {
            get { return Panel.Alignment; }
            set 
            { 
                Panel.Alignment = value;
                m_Box.Alignment = value;
            }
        }
        public override ExpansionFlags Expansion
        {
            get { return Panel.Expansion; }
            set
            {
                Panel.Expansion = value;
                m_Box.Expansion = value == ExpansionFlags.Fit ? value : ExpansionFlags.Expand;
            }
        }

        private readonly ImageBox m_Image;
        private readonly Text m_Text;
        private readonly ButtonBackground m_Background;
        private readonly BoxPanel m_Box;
        
        /// <summary>
        /// Text alignment.
        /// </summary>
        public AlignmentFlags TextAlignment { get { return m_Text.Alignment; } set { m_Text.Alignment = value; Invalidate(); } }
        
        /// <summary>
        /// Image alignment.
        /// </summary>
        public AlignmentFlags ImageAlignment { get { return m_Image.Alignment; } set { m_Image.Alignment = value; Invalidate(); } }

        /// <summary>
        /// Button elements orientation.
        /// </summary>
        public Orientation ButtonOrientation { get { return m_Box.Orientation; } set { m_Box.Orientation = value; Invalidate(); } }
        
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

        //public override void OnCanvasChanged(Canvas canvas)
        //{
        //    base.OnCanvasChanged(canvas);
        //    Layout(Skin);
        //}

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

        private bool m_Order;
        public bool TextFirst 
        { 
            get { return m_Order; } 
            set 
            { 
                if(value == m_Order)
                    return;
                m_Order = value;
                m_Box.SwapChildren(0, 1);
                m_Box.Invalidate();
            }
        } 
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
            get { return m_Background.IsDepressed; }
            set
            {
                if (m_Background.IsDepressed == value) 
                    return;
                m_Background.IsDepressed = value; 
                Redraw();
            }
        }

        /// <summary>
        /// Indicates whether the button is toggleable.
        /// </summary>
        public bool IsToggle { get { return m_Background.IsToggle; } set { m_Background.IsToggle = value; } }

        /// <summary>
        /// Determines the button's toggle state.
        /// </summary>
        public bool ToggleState
        {
            get { return m_Background.ToggleState; }
            set
            {
                if (!m_Background.IsToggle) return;
                if (m_Background.ToggleState == value) return;

                m_Background.ToggleState = value;

                if (Toggled != null)
                    Toggled.Invoke(this);

                if (m_Background.ToggleState)
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
        public Button()
            : base(new DockPanel())
        {
            m_Box = new BoxPanel(Orientation.Horizontal, ExpansionFlags.FitExpand);
            m_Background = new ButtonBackground() {Expansion = ExpansionFlags.Fill};
            m_Text = new Text();
            m_Text.Hide();
            m_Image = new ImageBox();
            m_Image.Hide();
            
            m_Box.AddChild(m_Image);
            m_Box.AddChild(m_Text);
            
            Panel.AddChild(m_Background);
            Panel.AddChild(m_Box);

            //SetSize(100, 20);
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
            if(string.IsNullOrEmpty(str))
                m_Text.Hide();
            else
                m_Text.Show();

            SizeToContents();
        }

        public virtual void SizeToContents()
        {
            if (Skin == null)
                return;
            PreLayout(Skin);
            InvalidateParent();
        }

        /// <summary>
        /// Toggles the button.
        /// </summary>
        public void Toggle()
        {
            ToggleState = !ToggleState;
        }

        /// <summary>
        /// "Clicks" the button.
        /// </summary>
        public void Press(IControl control = null)
        {
            OnClicked();
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
                if (IsHovered && IsDepressed)
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
        public void SetImage(String textureName, AlignmentFlags position = AlignmentFlags.NotSet)
        {
            var resMan = Engine.GetEngine().ResourceManager;
            var options = new ResourceLoadOptions { Name = textureName, AsynchronousLoad = false };
            var imageHandle = resMan.LoadResource<Image>(options);
            if (imageHandle.Id == 0)
                throw new Exception("Could not load GUI texture: " + textureName);
            SetImage(imageHandle, position);
        }

        /// <summary>
        /// Sets the button's image.
        /// </summary>
        /// <param name="imageHandle">Image bitmap. Set id to invalid to remove.</param>
        /// <param name="position">Determines image alignment.</param>
        public void SetImage(ResourceHandle<Image> imageHandle, AlignmentFlags position = AlignmentFlags.NotSet)
        {
            if(imageHandle.Id == ResourceHandle<Image>.Invalid)
            {
                m_Image.Hide();
                SizeToContents();
                return;
            }

            m_Image.Show();
            m_Image.SetImage(imageHandle);

            m_Image.SizeToContents();
            m_Image.Margin = Margin.Two;
            m_Image.Alignment = position;
            SizeToContents();
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
            OnMouseClickedLeft(x, y, true);
            if (DoubleClickedLeft != null)
                DoubleClickedLeft.Invoke(this);
        }
    }
}
