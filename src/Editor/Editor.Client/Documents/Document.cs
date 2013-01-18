using System;

namespace Flood.Editor
{
    /// <summary>
    /// Documents are an abstract concept used to decouple and data of
    /// the application from the views that interact with the user.
    /// </summary>
    public interface IDocument : IDisposable
    {
        /// <summary>
        /// Undo manager with the document undo history.
        /// </summary>
        IUndoManager UndoManager { get; }

        /// <summary>
        /// Path to the document.
        /// </summary>
        string Path { get; }

        /// <summary>
        /// Stores if the document has unsaved changes.
        /// </summary>
        bool HasUnsavedChanges { get; }
    }

    /// <summary>
    /// Base class to implement new kinds of documents.
    /// </summary>
    public abstract class Document : IDocument
    {
        public IUndoManager UndoManager { get; private set; }

        public string Path { get; private set; }

        public bool HasUnsavedChanges { get; set; }

        protected Document(IUndoManager undoManager, string path)
        {
            if (undoManager == null)
                throw new ArgumentNullException();

            Path = path;
            UndoManager = undoManager;

            UndoManager.OperationDone += OnUndoRedoEvent;
            UndoManager.OperationUndone += OnUndoRedoEvent;
        }

        public void Dispose()
        {
            UndoManager.OperationDone -= OnUndoRedoEvent;
            UndoManager.OperationUndone -= OnUndoRedoEvent;
        }

        public void OnUndoRedoEvent(object sender, IUndoOperation operation)
        {
            HasUnsavedChanges = UndoManager.HasUndoneOperations;
        }
    }
}
