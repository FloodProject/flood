using System;
using System.Collections.Generic;
using Flood;
using Flood.GUI;
using Flood.GUI.Controls;

namespace TextAddin.Controls
{
    public class VisualLineElement : Control
    {
        public TextRunProperties TextRunProperties { get; set; }

        public string Text { get; private set; }

        public int Offset { get; private set; }

        public int Length { get; private set;}

        /// <summary>
        /// Gets if this VisualLineElement can be split.
        /// </summary>
        public bool CanSplit
        {
            get { return true; }
        }

        public VisualLineElement(VisualLine visualLine, int offset, int length ) : base(visualLine)
        {
            MouseInputEnabled = true;
            TextRunProperties = visualLine.TextLayer.GlobalTextRunProperties;
            Offset = offset;
            Length = length;
            UpdateText();
        }

        private void UpdateText()
        {
            Text = ((VisualLine)Parent).TextLayer.Document.GetText(Offset, Length);
            var size = TextRenderer.MeasureText(Text, Skin.DefaultFont);
            SetSize((int)(size.X+0.5), (int)(size.Y+0.5));
            Invalidate();
            InvalidateParent();
        }

        protected override void Render(Flood.GUI.Skins.Skin skin)
        {
            skin.Renderer.DrawColor = TextRunProperties.Foreground;
            skin.Renderer.RenderText(skin.DefaultFont, new Vector2i(0,0), Text);
        }

        protected override void OnMouseClickedLeft(int x, int y, bool down)
        {
            base.OnMouseClickedRight(x, y, down);
            int index;
            if(TextRenderer.GetPositionTextIndex(Text, Skin.DefaultFont, x, out index))
                Console.WriteLine(Text[index]);
        }

        /// <summary>
        /// Splits the element.
        /// </summary>
        /// <param name="splitStartOffset">Position inside this element at which it should be broken</param>
        /// <param name="elements">The collection of line elements</param>
        /// <param name="elementIndex">The index at which this element is in the elements list.</param>
        public void Split(int splitStartOffset, IList<VisualLineElement> elements, int elementIndex)
        {
            if (splitStartOffset <= Offset || splitStartOffset >= Offset + Length)
                throw new ArgumentOutOfRangeException("splitStartOffset", splitStartOffset, "Value must be between " + (Offset + 1) + " and " + (Offset + Length - 1));
            if (elements == null)
                throw new ArgumentNullException("elements");
            if (elements[elementIndex] != this)
                throw new ArgumentException("Invalid elementIndex - couldn't find this element at the index");
            int part1Length = splitStartOffset - Offset;
            int part2Length = Length - part1Length;
            Length = part1Length;
            UpdateText();
            var splitPart = new VisualLineElement((VisualLine) Parent, splitStartOffset, part2Length);
            elements.Insert(elementIndex + 1, splitPart);
        }

    }
}
