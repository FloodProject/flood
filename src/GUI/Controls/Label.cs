using System;
using System.Drawing;
using Gwen.ControlInternal;

namespace Gwen.Control
{
    /// <summary>
    /// Static text label.
    /// </summary>
    public class Label : Base
    {
        private readonly Text m_Text;
        private Pos m_Align;
        private Padding m_TextPadding;
        private bool m_AutoSizeToContents;

        /// <summary>
        /// Text alignment.
        /// </summary>
        public Pos Alignment { get; set; }

        /// <summary>
        /// Text.
        /// </summary>
        public String Text { get; set; }

        /// <summary>
        /// Font.
        /// </summary>
        public Font Font { get; set; }

        /// <summary>
        /// Text color.
        /// </summary>
        public Color TextColor { get; set; }

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

        /// <summary>
        /// Determines if the control should autosize to its text.
        /// </summary>
        public bool AutoSizeToContents { get { return m_AutoSizeToContents; } set { m_AutoSizeToContents = value; Invalidate(); InvalidateParent(); } }

        /// <summary>
        /// Text padding.
        /// </summary>
        public Padding TextPadding { get { return m_TextPadding; } set { m_TextPadding = value; Invalidate(); InvalidateParent(); } }

        /// <summary>
        /// Initializes a new instance of the <see cref="Label"/> class.
        /// </summary>
        public Label()
        {
            m_Text = new Text();
            //m_Text.Font = Skin.DefaultFont;

            MouseInputEnabled = false;
            SetSize(100, 10);
            Alignment = Pos.Left | Pos.Top;

            m_AutoSizeToContents = false;
        }

        /// <summary>
        /// Returns index of the character closest to specified point (in canvas coordinates).
        /// </summary>
        /// <param name="x"></param>
        /// <param name="y"></param>
        /// <returns></returns>
        protected int GetClosestCharacter( int x, int y)
        { 
            return m_Text.GetClosestCharacter(m_Text.CanvasPosToLocal( new Point(x, y))); 
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

        /// <summary>
        /// Handler for text changed event.
        /// </summary>
        protected virtual void OnTextChanged()
        {}

        /// <summary>
        /// Lays out the control's interior according to alignment, padding, dock etc.
        /// </summary>
        /// <param name="skin">Skin to use.</param>
        protected override void Layout(Skin.Base skin)
        {
            base.Layout(skin);

            Pos align = m_Align;

            if (m_AutoSizeToContents)
                SizeToContents(skin);

            int x = m_TextPadding.Left + Padding.Left;
            int y = m_TextPadding.Top + Padding.Top;

            if (0 != (align & Pos.Right)) 
                x = Width - m_Text.Width - m_TextPadding.Right - Padding.Right;
            if (0 != (align & Pos.CenterH))
                x = (int)((m_TextPadding.Left + Padding.Left) + ((Width - m_Text.Width - m_TextPadding.Left - Padding.Left - m_TextPadding.Right - Padding.Right) * 0.5f));

            if (0 != (align & Pos.CenterV))
                y = (int)((m_TextPadding.Top + Padding.Top) + ((Height - m_Text.Height) * 0.5f) - m_TextPadding.Bottom - Padding.Bottom);
            if (0 != (align & Pos.Bottom)) 
                y = Height - m_Text.Height - m_TextPadding.Bottom - Padding.Bottom;

            m_Text.SetPosition(x, y);
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
            //if (m_AutoSizeToContents)
            //    SizeToContents();
            Invalidate();
            InvalidateParent();

            if (doEvents)
                OnTextChanged();
        }

        public virtual void SizeToContents(Skin.Base skin)
        {
            m_Text.SetPosition(m_TextPadding.Left + Padding.Left, m_TextPadding.Top + Padding.Top);
            m_Text.SizeToContents(skin);

            SetSize(m_Text.Width + Padding.Left + Padding.Right + m_TextPadding.Left + m_TextPadding.Right, 
                m_Text.Height + Padding.Top + Padding.Bottom + m_TextPadding.Top + m_TextPadding.Bottom);
            InvalidateParent();
        }

        /// <summary>
        /// Gets the coordinates of specified character.
        /// </summary>
        /// <param name="index">Character index.</param>
        /// <returns>Character coordinates (local).</returns>
        public virtual Point GetCharacterPosition(int index)
        {
            Point p = m_Text.GetCharacterPosition(index);
            return new Point(p.X + m_Text.X, p.Y + m_Text.Y);
        }

        /// <summary>
        /// Renders the control using specified skin.
        /// </summary>
        /// <param name="skin">Skin to use.</param>
        protected override void Render(Skin.Base skin)
        {
        }
    }
}
