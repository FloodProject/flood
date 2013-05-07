using System.Collections.Generic;
using System.ComponentModel.Design;
using System.Diagnostics;
using System.Linq;
using Flood;
using System;
using TextAddin.Document;
using TextAddin.Highlighting;
using TextAddin.Rendering;
using TextAddin.Utils;

namespace TextAddin.Controls
{
    public class TextLayer : Layer, ILineTracker, IServiceProvider
    {
        private readonly Dictionary<DocumentLine, VisualLine> lines;

        public TextRunProperties GlobalTextRunProperties { get; private set; }

        public TextLayer(TextView parent, TextDocument textDocument) : base(parent)
        {
            Document = textDocument;

            elementGenerators = new ObserveAddRemoveCollection<VisualLineElementGenerator>(ElementGenerator_Added, ElementGenerator_Removed);
            lineTransformers = new ObserveAddRemoveCollection<IVisualLineTransformer>(LineTransformer_Added, LineTransformer_Removed);

            heightTree = new HeightTree(textDocument,Skin.DefaultFont.Size);
            lines = new Dictionary<DocumentLine, VisualLine>();

            GlobalTextRunProperties = new TextRunProperties { Foreground = Color.Black};

            lineTransformers = new ObserveAddRemoveCollection<IVisualLineTransformer>(LineTransformer_Added,LineTransformer_Removed);

            HighlightingDefinition = HighlightingManager.DefaultHighlightingManager.Instance.GetDefinition("C#");

            var colorizer = new HighlightingColorizer(highlightingDefinition.MainRuleSet);
            lineTransformers.Add(colorizer);

            weakLineTracker = WeakLineTracker.Register(textDocument, this);

            RebuildDocument();
        }

        public Vector2 GetTextLocationPosition(TextLocation textLocation)
        {
            var documentLine = heightTree.GetLineByNumber(textLocation.Line);
            var line = lines[documentLine];
            return line.GetColumnBegining(textLocation.Column);
        }

        public TextLocation GetTextLocation(Vector2i position)
        {
            var documentLine = heightTree.GetLineByVisualPosition(position.Y);
            var line = lines[documentLine];
            return line.GetTextLocation(position.X);
        }

        public bool GetIsCollapsed(int lineNumber)
        {
            return heightTree.GetIsCollapsed(lineNumber);
        }

        protected override void Layout(Flood.GUI.Skins.Skin skin)
        {
            double y = 0;
            foreach (var control in Children)
            {
                if (!control.IsVisible)
                    continue;

                control.SetPosition(0,(int)y);
                y += heightTree.DefaultLineHeight;
            }
        }


        #region Document Property

        HeightTree heightTree;

        /// <summary>
        /// Gets/Sets the document displayed by the text editor.
        /// </summary>
        public TextDocument Document { get; private set; }
        
        /// <summary>
        /// Occurs when the document property has changed.
        /// </summary>
        public event EventHandler DocumentChanged;
        
        void OnDocumentChanged(TextDocument oldValue, TextDocument newValue)
        {
            if (DocumentChanged != null)
                DocumentChanged(this, EventArgs.Empty);
        }
        
        #endregion


        #region ElementGenerators+LineTransformers Properties
        readonly ObserveAddRemoveCollection<VisualLineElementGenerator> elementGenerators;
        
        /// <summary>
        /// Gets a collection where element generators can be registered.
        /// </summary>
        public IList<VisualLineElementGenerator> ElementGenerators {
            get { return elementGenerators; }
        }
        
        void ElementGenerator_Added(VisualLineElementGenerator generator)
        {
            ConnectToTextView(generator);
            Redraw();
        }
        
        void ElementGenerator_Removed(VisualLineElementGenerator generator)
        {
            DisconnectFromTextView(generator);
            Redraw();
        }
        
        readonly ObserveAddRemoveCollection<IVisualLineTransformer> lineTransformers;
        
        /// <summary>
        /// Gets a collection where line transformers can be registered.
        /// </summary>
        public IList<IVisualLineTransformer> LineTransformers {
            get { return lineTransformers; }
        }
        
        void LineTransformer_Added(IVisualLineTransformer lineTransformer)
        {
            ConnectToTextView(lineTransformer);
            Redraw();
        }
        
        void LineTransformer_Removed(IVisualLineTransformer lineTransformer)
        {
            DisconnectFromTextView(lineTransformer);
            Redraw();
        }
        #endregion


        #region Service Provider
        readonly ServiceContainer services = new ServiceContainer();
        
        /// <summary>
        /// Gets a service container used to associate services with the text view.
        /// </summary>
        public ServiceContainer Services {
            get { return services; }
        }
        
        object IServiceProvider.GetService(Type serviceType)
        {
            return services.GetService(serviceType);
        }
        
        void ConnectToTextView(object obj)
        {
            ITextLayerConnect c = obj as ITextLayerConnect;
            if (c != null)
                c.AddToTextLayer(this);
        }
        
        void DisconnectFromTextView(object obj)
        {
            ITextLayerConnect c = obj as ITextLayerConnect;
            if (c != null)
                c.RemoveFromTextLayer(this);
        }
        #endregion


        #region ILineTracker

        private WeakLineTracker weakLineTracker;

        public void BeforeRemoveLine(DocumentLine documentLine)
        {
            var line = lines[documentLine];
            RemoveChild(line,true);
            lines.Remove(documentLine);
        }

        public void SetLineLength(DocumentLine line, int newTotalLength)
        {}

        public void LineInserted(DocumentLine insertionPos, DocumentLine newDocLine)
        {
            var prevLine = lines[insertionPos];
            var line = new VisualLine(this, newDocLine);
            line.BringNextToControl(prevLine,true);
            lines.Add(newDocLine,line);
        }

        public void RebuildDocument()
        {
            lines.Clear();
            DeleteAllChildren();

            foreach (var docLine in Document.Lines)
            {
                var line = new VisualLine(this, docLine);
                lines.Add(docLine,line);
            }
        }

        #endregion

        
        #region Syntax highlighting

        IVisualLineTransformer colorizer;

        private IHighlightingDefinition highlightingDefinition;

        public IHighlightingDefinition HighlightingDefinition
        {
            get { return highlightingDefinition; }
            set 
            { 
                if(highlightingDefinition != value)
                {
                    highlightingDefinition = value;
                    OnSyntaxHighlightingChanged(value);
                }
            }
        }

        void OnSyntaxHighlightingChanged(IHighlightingDefinition newValue)
        {
            if (colorizer != null) {
                LineTransformers.Remove(colorizer);
                colorizer = null;
            }
            if (newValue != null) {
                colorizer = CreateColorizer(newValue);
                LineTransformers.Insert(0, colorizer);
            }
        }
        
        /// <summary>
        /// Creates the highlighting colorizer for the specified highlighting definition.
        /// Allows derived classes to provide custom colorizer implementations for special highlighting definitions.
        /// </summary>
        /// <returns></returns>
        protected virtual IVisualLineTransformer CreateColorizer(IHighlightingDefinition highlightingDefinition)
        {
            if (highlightingDefinition == null)
                throw new ArgumentNullException("highlightingDefinition");
            return new HighlightingColorizer(highlightingDefinition.MainRuleSet);
        }
        #endregion


        #region InitVisualLine

        
        public VisualLine InitVisualLine(VisualLine visualLine)
        {
            Debug.WriteLine("Initializing line " + visualLine.DocumentLine.LineNumber);
            
            visualLine.ConstructVisualElements(elementGenerators.ToArray());
            
            visualLine.RunTransformers(visualLine, lineTransformers.ToArray());

            return visualLine;
        }
        #endregion


        #region Visual Lines (fields and properties)
        
        /// <summary>
        /// Occurs when the TextView is about to be measured and will regenerate its visual lines.
        /// This event may be used to mark visual lines as invalid that would otherwise be reused.
        /// </summary>
        public event EventHandler<VisualLineConstructionStartEventArgs> VisualLineConstructionStarting;
        
        /// <summary>
        /// Occurs when the TextView was measured and changed its visual lines.
        /// </summary>
        public event EventHandler VisualLinesChanged;
        
        #endregion
        
    }
}
