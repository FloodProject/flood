using Flood.GUIv2.ControlInternal;
using System;
using Flood.GUIv2.Panels;
using Flood.GUIv2.Panels.Layout;
using Flood.GUIv2.Skins;

namespace Flood.GUIv2.Controls
{
    /// <summary>
    /// Static text label.
    /// </summary>
    public class Label : CompositeControl
    {
        private readonly Text m_Text;
        //todo: this has been replaced by the the panel fit, when all controls derived from this are fixed remove it
        private bool m_AutoSizeToContents;

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
        /// Determines if the control should autosize to its text.
        /// </summary>
        public bool AutoSizeToContents { get { return m_AutoSizeToContents; } set { m_AutoSizeToContents = value; Invalidate(); } }

        /// <summary>
        /// Text margin.
        /// </summary>
        public Margin TextMargin { get { return m_Text.Margin; } set { m_Text.Margin = value; Invalidate(); } }

        /// <summary>
        /// Initializes a new instance of the <see cref="Label"/> class.
        /// </summary>
        public Label()
            : base(new DockPanel())

        {
            m_Text = new Text();
            Panel.AddChild(m_Text);
            //m_Text.Font = Skin.DefaultFont;

            MouseInputEnabled = false;
            SetSize(100, 10);
            TextAlignment = AlignmentFlags.Left | AlignmentFlags.Top;

            m_AutoSizeToContents = true;
        }

        /// <summary>
        /// Returns index of the character closest to specified point (in canvas coordinates).
        /// </summary>
        /// <param name="x"></param>
        /// <param name="y"></param>
        /// <returns></returns>
        protected int GetClosestCharacter(int x, int y)
        { 
            return m_Text.GetClosestCharacter(m_Text.CanvasPosToLocal(new Vector2i(x, y))); 
        }

        /// <summary>
        /// Sets the position of the internal text control.
        /// </summary>
        /// <param name="x"></param>
        /// <param name="y"></param>
        protected void SetTextPosition(int x, int y)
        {
            m_Text.SetPosition(x, y);
        }

        //todo: remove this when controls derived form this no longer use it
        /// <summary>
        /// Handler for text changed event.
        /// </summary>
        protected virtual void OnTextChanged()
        {}

        //todo: remove virtual from this when controls derived form this no longer use it
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

            if (doEvents)
                OnTextChanged();
        }

        //public virtual void SizeToContents()
        //{
        //    m_Text.SetPosition(m_TextPadding.Left + Padding.Left, m_TextPadding.Top + Padding.Top);
        //    m_Text.SizeToContents();

        //    SetSize(m_Text.Width + Padding.Left + Padding.Right + m_TextPadding.Left + m_TextPadding.Right, 
        //        m_Text.Height + Padding.Top + Padding.Bottom + m_TextPadding.Top + m_TextPadding.Bottom);
        //    InvalidateParent();
        //}

        public virtual void SizeToContents()
        {
            if(Skin == null)
                return;
            Layout(Skin);
            InvalidateParent();
        }

        /// <summary>
        /// Gets the coordinates of specified character.
        /// </summary>
        /// <param name="index">Character index.</param>
        /// <returns>Character coordinates (local).</returns>
        public virtual Vector2 GetCharacterPosition(int index)
        {
            var p = m_Text.GetCharacterPosition(index);
            return new Vector2(p.X + m_Text.X, p.Y + m_Text.Y);
        }

    }
}
