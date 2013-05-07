// Copyright (c) AlphaSierraPapa for the SharpDevelop Team (for details please see \doc\copyright.txt)
// This code is distributed under the GNU LGPL (for details please see \doc\license.txt)

using System;
using System.Diagnostics;
using TextAddin.Controls;
using TextAddin.Document;
using TextAddin.Rendering;

namespace TextAddin.Highlighting
{
    /// <summary>
    /// A colorizes that interprets a highlighting rule set and colors the document accordingly.
    /// </summary>
    public class HighlightingColorizer : ColorizingTransformer
    {
        readonly HighlightingRuleSet ruleSet;
        
        /// <summary>
        /// Creates a new HighlightingColorizer instance.
        /// </summary>
        /// <param name="ruleSet">The root highlighting rule set.</param>
        public HighlightingColorizer(HighlightingRuleSet ruleSet)
        {
            if (ruleSet == null)
                throw new ArgumentNullException("ruleSet");
            this.ruleSet = ruleSet;
        }
        
        void OnDocumentChanged(TextLayer textLayer)
        {
            // remove existing highlighter, if any exists
            textLayer.Services.RemoveService(typeof(IHighlighter));
            textLayer.Services.RemoveService(typeof(DocumentHighlighter));
            
            TextDocument document = textLayer.Document;
            if (document != null) {
                IHighlighter highlighter = CreateHighlighter(textLayer, document);
                textLayer.Services.AddService(typeof(IHighlighter), highlighter);
                // for backward compatiblity, we're registering using both the interface and concrete types
                if (highlighter is DocumentHighlighter)
                    textLayer.Services.AddService(typeof(DocumentHighlighter), highlighter);
            }
        }
        
        /// <summary>
        /// Creates the IHighlighter instance for the specified text document.
        /// </summary>
        protected virtual IHighlighter CreateHighlighter(TextLayer textLayer, TextDocument document)
        {
            return new TextLayerDocumentHighlighter(this, textLayer, ruleSet);
        }
        
        /// <inheritdoc/>
        protected override void OnAddToTextLayer(TextLayer textLayer)
        {
            base.OnAddToTextLayer(textLayer);
            OnDocumentChanged(textLayer);
        }
        
        /// <inheritdoc/>
        protected override void OnRemoveFromTextLayer(TextLayer textLayer)
        {
            base.OnRemoveFromTextLayer(textLayer);
            textLayer.Services.RemoveService(typeof(IHighlighter));
            textLayer.Services.RemoveService(typeof(DocumentHighlighter));
        }
        
        int lineNumberBeingColorized;

        /// <inheritdoc/>
        protected override void Colorize(VisualLine line)
        {
            IHighlighter highlighter = line.TextLayer.Services.GetService(typeof(IHighlighter)) as IHighlighter;
            if (highlighter != null) {
                lineNumberBeingColorized = line.DocumentLine.LineNumber;
                HighlightedLine hl = highlighter.HighlightLine(lineNumberBeingColorized);
                lineNumberBeingColorized = 0;
                foreach (HighlightedSection section in hl.Sections) {
                    if (IsEmptyColor(section.Color))
                        continue;

                    ChangeVisualElements(section.Offset, section.Offset + section.Length,
                                   visualLineElement => ApplyColorToElement(visualLineElement, section.Color));
                }
            }
        }
        
        /// <summary>
        /// Gets whether the color is empty (has no effect on a VisualLineTextElement).
        /// For example, the C# "Punctuation" is an empty color.
        /// </summary>
        bool IsEmptyColor(HighlightingColor color)
        {
            if (color == null)
                return true;
            return color.Background == null && color.Foreground == null
                && color.FontStyle == null && color.FontWeight == null;
        }
        
        /// <summary>
        /// Applies a highlighting color to a visual line element.
        /// </summary>
        protected virtual void ApplyColorToElement(VisualLineElement element, HighlightingColor color)
        {
            element.TextRunProperties = color.Apply(element.TextRunProperties);
        }
        
        /// <summary>
        /// This class is responsible for telling the TextLayer to redraw lines when the highlighting state has changed.
        /// </summary>
        /// <remarks>
        /// Creation of a VisualLine triggers the syntax highlighter (which works on-demand), so it says:
        /// Hey, the user typed "/*". Don't just recreate that line, but also the next one
        /// because my highlighting state (at end of line) changed!
        /// </remarks>
        sealed class TextLayerDocumentHighlighter : DocumentHighlighter
        {
            readonly HighlightingColorizer colorizer;
            readonly TextLayer textLayer;
            
            public TextLayerDocumentHighlighter(HighlightingColorizer colorizer, TextLayer textLayer, HighlightingRuleSet baseRuleSet)
                : base(textLayer.Document, baseRuleSet)
            {
                Debug.Assert(colorizer != null);
                Debug.Assert(textLayer != null);
                this.colorizer = colorizer;
                this.textLayer = textLayer;
            }
            
            protected override void OnHighlightStateChanged(DocumentLine line, int lineNumber)
            {
                base.OnHighlightStateChanged(line, lineNumber);
                if (colorizer.lineNumberBeingColorized != lineNumber) {
                    // Ignore notifications for any line except the one we're interested in.
                    // This improves the performance as Redraw() can take quite some time when called repeatedly
                    // while scanning the document (above the visible area) for highlighting changes.
                    return;
                }
                if (textLayer.Document != this.Document) {
                    // May happen if document on text view was changed but some user code is still using the
                    // existing IHighlighter instance.
                    return;
                }
                
                // The user may have inserted "/*" into the current line, and so far only that line got redrawn.
                // So when the highlighting state is changed, we issue a redraw for the line immediately below.
                // If the highlighting state change applies to the lines below, too, the construction of each line
                // will invalidate the next line, and the construction pass will regenerate all lines.
                
                Debug.WriteLine("OnHighlightStateChanged forces redraw of line " + (lineNumber + 1));
                
                // If the VisualLine construction is in progress, we have to avoid sending redraw commands for
                // anything above the line currently being constructed.
                // It takes some explanation to see why this cannot happen.
                // VisualLines always get constructed from top to bottom.
                // Each VisualLine construction calls into the highlighter and thus forces an update of the
                // highlighting state for all lines up to the one being constructed.
                
                // To guarantee that we don't redraw lines we just constructed, we need to show that when
                // a VisualLine is being reused, the highlighting state at that location is still up-to-date.
                
                // This isn't exactly trivial and the initial implementation was incorrect in the presence of external document changes
                // (e.g. split view).
                
                // For the first line in the view, the TextLayer.VisualLineConstructionStarting event is used to check that the
                // highlighting state is up-to-date. If it isn't, this method will be executed, and it'll mark the first line
                // in the view as requiring a redraw. This is safely possible because that event occurs before any lines are reused.
                
                // Once we take care of the first visual line, we won't get in trouble with other lines due to the top-to-bottom
                // construction process.
                
                // We'll prove that: if line N is being reused, then the highlighting state is up-to-date until (end of) line N-1.
                
                // Start of induction: the first line in view is reused only if the highlighting state was up-to-date
                // until line N-1 (no change detected in VisualLineConstructionStarting event).
                
                // Induction step:
                // If another line N+1 is being reused, then either
                //     a) the previous line (the visual line containing document line N) was newly constructed
                // or  b) the previous line was reused
                // In case a, the construction updated the highlighting state. This means the stack at end of line N is up-to-date.
                // In case b, the highlighting state at N-1 was up-to-date, and the text of line N was not changed.
                //   (if the text was changed, the line could not have been reused).
                // From this follows that the highlighting state at N is still up-to-date.
                
                // The above proof holds even in the presence of folding: folding only ever hides text in the middle of a visual line.
                // Our Colorize-override ensures that the highlighting state is always updated for the LastDocumentLine,
                // so it will always invalidate the next visual line when a folded line is constructed
                // and the highlighting stack has changed.
                
                throw new NotImplementedException();
                //textLayer.Redraw(line.NextLine, DispatcherPriority.Normal);
                
                /*
                 * Meta-comment: "why does this have to be so complicated?"
                 * 
                 * The problem is that I want to re-highlight only on-demand and incrementally;
                 * and at the same time only repaint changed lines.
                 * So the highlighter and the VisualLine construction both have to run in a single pass.
                 * The highlighter must take care that it never touches already constructed visual lines;
                 * if it detects that something must be redrawn because the highlighting state changed,
                 * it must do so early enough in the construction process.
                 * But doing it too early means it doesn't have the information necessary to re-highlight and redraw only the desired parts.
                 */
            }
        }
    }
}
