using System;
using Flood;
using Flood.Editor.Client;
using Flood.GUI.Controls;

namespace TextAddin.Render
{
    class TextLineElement : Control
    {
        public TextProperties TextProperties { get; set; }

        private string text;

        public string Text
        {
            get { return text; }
            set
            {
                text = value;
                SizeToText();
            }
        }

        public TextLineElement(TextLine parent) : base(parent)
        {
            MouseInputEnabled = true;
        }

        private void SizeToText()
        {
            var size = Skin.Renderer.MeasureText(Skin.DefaultFont, Text);
            SetSize(size.X+100, size.Y);
            Invalidate();
            InvalidateParent();
        }

        protected override void Render(Flood.GUI.Skins.Skin skin)
        {
            skin.Renderer.DrawColor = TextProperties.TextColor;
            skin.Renderer.RenderText(skin.DefaultFont,new Vector2i(0,0), Text);
        }

        protected override void OnMouseClickedLeft(int x, int y, bool down)
        {
            base.OnMouseClickedRight(x, y, down);
            int index;
            if(TextRenderer.GetPositionTextIndex(text, Skin.DefaultFont, x, out index))
                Console.WriteLine(text[index]);
        }
    }
}
