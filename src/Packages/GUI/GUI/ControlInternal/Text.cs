//#define DEBUG_TEXT_MEASURE

using System;
using Flood.GUI.Controls;

namespace Flood.GUI.ControlInternal
{
    /// <summary>
    /// Displays text. Always sized to contents.
    /// </summary>
    public class Text : Control
    {
        private String m_String;
        private Font m_Font;

        /// <summary>
        /// Font used to display the text.
        /// </summary>
        /// <remarks>
        /// The font is not being disposed by this class.
        /// </remarks>
        public Font Font
        {
            get { return m_Font; }
            set
            {
                m_Font = value;
                SizeToContents();
            }
        }

        /// <summary>
        /// Text to display.
        /// </summary>
        public String String
        {
            get { return m_String; }
            set
            {
                m_String = value;
                SizeToContents();
            }
        }

        /// <summary>
        /// Text color.
        /// </summary>
        public Color TextColor { get; set; }

        /// <summary>
        /// Determines whether the control should be automatically resized to fit the text.
        /// </summary>
        //public bool AutoSizeToContents { get; set; } // [omeg] added

        /// <summary>
        /// Text length in characters.
        /// </summary>
        public int Length { get { return String.Length; } }

        /// <summary>
        /// Text color override - used by tooltips.
        /// </summary>
        public Color TextColorOverride { get; set; }

        /// <summary>
        /// Text override - used to display different string.
        /// </summary>
        public String TextOverride { get; set; }

        /// <summary>
        /// Initializes a new instance of the <see cref="Text"/> class.
        /// </summary>
        /// <param name="parent">Parent control.</param>
        public Text(Control parent)
            : base(parent)
        {
            m_Font = Skin.DefaultFont;
            m_String = string.Empty;
            TextColor = Skin.Colors.Label.Default;
            MouseInputEnabled = false;
            TextColorOverride = new Color(255, 255, 255, 0); // A==0, override disabled
        }

        /// <summary>
        /// Renders the control using specified skin.
        /// </summary>
        /// <param name="skin">Skin to use.</param>
        protected override void Render(Skins.Skin skin)
        {
            if (Length == 0 || Font == null) return;

            if (TextColorOverride.A == 0)
                skin.Renderer.DrawColor = TextColor;
            else
                skin.Renderer.DrawColor = TextColorOverride;

            skin.Renderer.RenderText(Font, new Vector2i(0,0), TextOverride ?? String);

            #if DEBUG_TEXT_MEASURE
            {
                Point lastPos = Point.Empty;

                for (int i = 0; i < m_String.Length + 1; i++)
                {
                    String sub = (TextOverride ?? String).Substring(0, i);
                    Point p = Skin.Renderer.MeasureText(Font, sub);

                    Rectangle rect = new Rectangle();
                    rect.Location = lastPos;
                    rect.Size = new Size(p.X - lastPos.X, p.Y);
                    skin.Renderer.DrawColor = Color.FromArgb(64, 0, 0, 0);
                    skin.Renderer.DrawLinedRect(rect);

                    lastPos = new Point(rect.Right, 0);
                }
            }
            #endif
        }

        /// <summary>
        /// Lays out the control's interior according to alignment, padding, dock etc.
        /// </summary>
        /// <param name="skin">Skin to use.</param>
        protected override void Layout(Skins.Skin skin)
        {
            SizeToContents();
            base.Layout(skin);
        }

        /// <summary>
        /// Handler invoked when control's scale changes.
        /// </summary>
        protected override void OnScaleChanged()
        {
            Invalidate();
        }

        /// <summary>
        /// Sizes the control to its contents.
        /// </summary>
        public void SizeToContents()
        {
            if (String == null)
                return;

            if (Font == null)
            {
                throw new InvalidOperationException("Text.SizeToContents() - No Font!!\n");
            }

            var p = new Vector2(1, Font.Size);

            if (Length > 0)
            {
                p = Skin.Renderer.MeasureText(Font, TextOverride ?? String);
            }

            if (p.X == Width && p.Y == Height)
                return;

            SetSize((int)(p.X+0.5), (int)(p.Y+0.5));
            Invalidate();
            InvalidateParent();
        }

        /// <summary>
        /// Gets the coordinates of specified character in the text.
        /// </summary>
        /// <param name="index">Character index.</param>
        /// <returns>Character position in local coordinates.</returns>
        public Vector2 GetCharacterPosition(int index)
        {
            if (Length == 0 || index == 0)
            {
                return new Vector2(0, 0);
            }

            String sub = (TextOverride ?? String).Substring(0, index);
            var p = Skin.Renderer.MeasureText(Font, sub);

            return p;
        }

        /// <summary>
        /// Searches for a character closest to given point.
        /// </summary>
        /// <param name="p">Point.</param>
        /// <returns>Character index.</returns>
        public int GetClosestCharacter(Vector2i p)
        {
            float distance = MaxCoord;
            int c = 0;

            for (int i = 0; i < String.Length + 1; i++)
            {
                var cp = GetCharacterPosition(i);
                var dist = Math.Abs(cp.X - p.X); // TODO: handle multiline

                if (dist > distance) 
                    continue;

                distance = dist;
                c = i;
            }

            return c;
        }
    }
}
