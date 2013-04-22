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

        public Vector2 GetColumnBegining(int column)
        {
            var text = TextLayer.TextView.TextDocument.GetText(DocumentLine.Offset, DocumentLine.Length);
            var size = TextRenderer.MeasureText(text.Substring(0,column-1),Skin.DefaultFont);

            return new Vector2(Bounds.X+size.X,Bounds.Y);
        }

        public TextLocation GetTextLocation(float x)
        {
            var text = TextLayer.TextView.TextDocument.GetText(DocumentLine.Offset, DocumentLine.Length);

            int index;
            TextRenderer.GetPositionTextIndex(text, Skin.DefaultFont, x, out index);

            Console.WriteLine(index);

            //Line and Column indexes start at 1
            return new TextLocation(DocumentLine.LineNumber,index+1);
        }
    }
}
