// Copyright (c) AlphaSierraPapa for the SharpDevelop Team (for details please see \doc\copyright.txt)
// This code is distributed under the GNU LGPL (for details please see \doc\license.txt)

using System;
using System.Collections.Generic;
using TextAddin.Controls;

namespace TextAddin.Rendering
{
    /// <summary>
    /// Base class for <see cref="IVisualLineTransformer"/> that helps
    /// splitting visual elements so that colors (and other text properties) can be easily assigned
    /// to individual words/characters.
    /// </summary>
    public abstract class ColorizingTransformer : IVisualLineTransformer, ITextLayerConnect
    {
        /// <summary>
        /// Gets the list of elements currently being transformed.
        /// </summary>
        protected IList<VisualLineElement> CurrentElements { get; private set; }
        
        /// <summary>
        /// <see cref="IVisualLineTransformer.Transform"/> implementation.
        /// Sets <see cref="CurrentElements"/> and calls <see cref="Colorize"/>.
        /// </summary>
        public void Transform(VisualLine line, IList<VisualLineElement> elements)
        {
            if (elements == null)
                throw new ArgumentNullException("elements");
            if (this.CurrentElements != null)
                throw new InvalidOperationException("Recursive Transform() call");
            this.CurrentElements = elements;
            try {
                Colorize(line);
            } finally {
                this.CurrentElements = null;
            }
        }
        
        /// <summary>
        /// Performs the colorization.
        /// </summary>
        protected abstract void Colorize(VisualLine line);
        
        /// <summary>
        /// Changes visual element properties.
        /// This method accesses <see cref="CurrentElements"/>, so it must be called only during
        /// a <see cref="Transform"/> call.
        /// This method splits <see cref="VisualLineElement"/>s as necessary to ensure that the region
        /// can be colored by setting the <see cref="VisualLineElement.TextRunProperties"/> of whole elements,
        /// and then calls the <paramref name="action"/> on all elements in the region.
        /// </summary>
        /// <param name="startOffset">Start offset of the region to change</param>
        /// <param name="endOffset">End offset of the region to change</param>
        /// <param name="action">Action that changes an individual <see cref="VisualLineElement"/>.</param>
        protected void ChangeVisualElements(int startOffset, int endOffset, Action<VisualLineElement> action)
        {
            if (action == null)
                throw new ArgumentNullException("action");
            for (int i = 0; i < CurrentElements.Count; i++) {
                VisualLineElement e = CurrentElements[i];
                
                if (e.Offset > endOffset)
                    break;

                if (e.Offset < startOffset && e.Offset + e.Length > startOffset)
                {
                    if (e.CanSplit) {
                        e.Split(startOffset, CurrentElements, i--);
                        continue;
                    }
                }
                if (e.Offset < endOffset && e.Offset + e.Length > endOffset) {
                    if (e.CanSplit) {
                        e.Split(endOffset, CurrentElements, i--);
                        continue;
                    }
                } 

                if(e.Offset == startOffset && e.Offset + e.Length == endOffset)
                    action(e);
            }
        }
        
        /// <summary>
        /// Called when added to a text view.
        /// </summary>
        protected virtual void OnAddToTextLayer(TextLayer textLayer)
        {
        }
        
        /// <summary>
        /// Called when removed from a text view.
        /// </summary>
        protected virtual void OnRemoveFromTextLayer(TextLayer textLayer)
        {
        }
        
        void ITextLayerConnect.AddToTextLayer(TextLayer textLayer)
        {
            OnAddToTextLayer(textLayer);
        }
        
        void ITextLayerConnect.RemoveFromTextLayer(TextLayer textLayer)
        {
            OnRemoveFromTextLayer(textLayer);
        }
    }
}
