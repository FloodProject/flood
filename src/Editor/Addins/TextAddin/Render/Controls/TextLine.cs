using System;
using Flood;
using Flood.Editor.Client;
using Flood.GUI.Controls;
using ICSharpCode.AvalonEdit.Document;

namespace TextAddin.Render
{
    internal class TextLine : Control
    {
        public TextLayer TextLayer
        {
            get { return (TextLayer) Parent; }
        }

        public DocumentLine DocumentLine { get; private set; }

        public TextLine(TextLayer parent, DocumentLine documentLine) : base(parent)
        {
            DocumentLine = documentLine;
            BuildLineElements();

            documentLine.TextChanged += (sender, args) => BuildLineElements();
        }

        private void BuildLineElements()
        {
            DeleteAllChildren();

            var text = TextLayer.TextView.TextDocument.GetText(DocumentLine.Offset, DocumentLine.Length);

            var textLineElement = new TextLineElement(this);
            textLineElement.Text = text;
            textLineElement.TextProperties = new TextProperties {TextColor = Color.Red};

            Invalidate();
        }

        protected override void Layout(Flood.GUI.Skins.Skin skin)
        {
            base.Layout(skin);
            SizeToChildren();
        }

        public Vector2 SnapPositionToText(Vector2 pos)
        {
            var text = TextLayer.TextView.TextDocument.GetText(DocumentLine.Offset, DocumentLine.Length);

            int index;
            TextRenderer.GetPositionTextIndex(text, Skin.DefaultFont, pos.X, out index);

            var size = TextRenderer.MeasureText(text.Substring(0,index),Skin.DefaultFont);

            return new Vector2(Bounds.X+size.X,Bounds.Y);
        }
    }
}
