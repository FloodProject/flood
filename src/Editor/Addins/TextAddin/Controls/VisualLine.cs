using System;
using System.Collections.Generic;
using System.Diagnostics;
using Flood;
using Flood.Editor.Client;
using Flood.GUI.Controls;
using TextAddin.Document;
using TextAddin.Rendering;

namespace TextAddin.Controls
{
    public class VisualLine : Control
    {
        enum LifetimePhase : byte
        {
            Generating,
            Transforming,
            Live,
            Disposed
        }

        LifetimePhase phase;

        public TextLayer TextLayer
        {
            get { return (TextLayer) Parent; }
        }

        public DocumentLine DocumentLine { get; private set; }

        private List<VisualLineElement> elements;

        public VisualLine(TextLayer parent, DocumentLine documentLine) : base(parent)
        {
            DocumentLine = documentLine;
            elements = new List<VisualLineElement>();

            Init();

            documentLine.TextChanged += OnTextChanged;
        }

        public override void Dispose()
        {
            base.Dispose();
            DocumentLine.TextChanged -= OnTextChanged;
        }

        private void OnTextChanged(object sender, EventArgs eventArgs)
        {
            Init();
        }

        private void Init()
        {
            DeleteAllChildren();

            phase = LifetimePhase.Generating;
            TextLayer.InitVisualLine(this);
        }


        protected override void Layout(Flood.GUI.Skins.Skin skin)
        {
            base.Layout(skin);
            var currentX = 0;
            foreach (var child in Children)
            {
                child.X = currentX;
                currentX += child.Width;
            }
            SizeToChildren();
        }

        public Vector2 GetColumnBegining(int column)
        {
            var text = TextLayer.Document.GetText(DocumentLine.Offset, DocumentLine.Length);
            var size = TextRenderer.MeasureText(text.Substring(0,column-1),Skin.DefaultFont);

            return new Vector2(Bounds.X+size.X,Bounds.Y);
        }

        public TextLocation GetTextLocation(float x)
        {
            var text = TextLayer.Document.GetText(DocumentLine.Offset, DocumentLine.Length);

            int index;
            TextRenderer.GetPositionTextIndex(text, Skin.DefaultFont, x, out index);

            Console.WriteLine(index);

            //Line and Column indexes start at 1
            return new TextLocation(DocumentLine.LineNumber,index+1);
        }

        internal void ConstructVisualElements(VisualLineElementGenerator[] generators)
        {
            Debug.Assert(phase == LifetimePhase.Generating);
            foreach (VisualLineElementGenerator g in generators) {
                g.StartGeneration(this);
            }
            elements.Clear();
            PerformVisualElementConstruction(generators);
            foreach (VisualLineElementGenerator g in generators) {
                g.FinishGeneration();
            }
            phase = LifetimePhase.Transforming;
        }

        void PerformVisualElementConstruction(VisualLineElementGenerator[] generators)
        {
            TextDocument document = TextLayer.Document;
            int offset = DocumentLine.Offset;
            int currentLineEnd = offset + DocumentLine.Length;
            int askInterestOffset = 0; // 0 or 1
            while (offset + askInterestOffset <= currentLineEnd) {
                int textLineEndOffset = currentLineEnd;
                foreach (VisualLineElementGenerator g in generators) {
                    g.cachedInterest = g.GetFirstInterestedOffset(offset + askInterestOffset);
                    if (g.cachedInterest != -1) {
                        if (g.cachedInterest < offset)
                            throw new ArgumentOutOfRangeException(g.GetType().Name + ".GetFirstInterestedOffset",
                                                                  g.cachedInterest,
                                                                  "GetFirstInterestedOffset must not return an offset less than startOffset. Return -1 to signal no interest.");
                        if (g.cachedInterest < textLineEndOffset)
                            textLineEndOffset = g.cachedInterest;
                    }
                }
                Debug.Assert(textLineEndOffset >= offset);
                if (textLineEndOffset > offset) {
                    int textElementLength = textLineEndOffset - offset;
                    var element = new VisualLineElement(this, offset, textElementLength);
                    elements.Add(element);
                    offset = textLineEndOffset;
                }
                // If no elements constructed / only zero-length elements constructed:
                // do not asking the generators again for the same location (would cause endless loop)
                askInterestOffset = 1;
                foreach (VisualLineElementGenerator g in generators) {
                    if (g.cachedInterest == offset) {
                        VisualLineElement element = g.ConstructElement(offset);
                        if (element != null) {
                            elements.Add(element);
                            if (element.Text.Length > 0) {
                                // a non-zero-length element was constructed
                                askInterestOffset = 0;
                                offset += element.Text.Length;
                                if (offset > currentLineEnd) {
                                    DocumentLine newEndLine = document.GetLineByOffset(offset);
                                    currentLineEnd = newEndLine.Offset + newEndLine.Length;
                                    if (currentLineEnd < offset) {
                                        throw new InvalidOperationException(
                                            "The VisualLineElementGenerator " + g.GetType().Name +
                                            " produced an element which ends within the line delimiter");
                                    }
                                }
                                break;
                            }
                        }
                    }
                }
            }
        }

        internal void RunTransformers(VisualLine line, IVisualLineTransformer[] transformers)
        {
            Debug.Assert(phase == LifetimePhase.Transforming);
            foreach (IVisualLineTransformer transformer in transformers) {
                transformer.Transform(line, elements);
            }
            phase = LifetimePhase.Live;
        }
    }
}
