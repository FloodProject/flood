using System;
using System.Collections.Generic;

namespace Flood.Editor
{
    /// <summary>
    /// Represents an operation that can be done and undone.
    /// </summary>
    public interface IUndoOperation
    {
        /// <summary>
        /// Does the operation.
        /// </summary>
        void Do();

        /// <summary>
        /// Undoes the operation.
        /// </summary>
        void Undo();
    }

    /// <summary>
    /// Represents an undo/redo system by keeping an history of operations.
    /// </summary>
    public interface IUndoManager
    {
        /// <summary>
        /// Read-only access to operations that have been done.
        /// </summary>
        IEnumerable<IUndoOperation> Operations { get; }

        /// <summary>
        /// Read-only access to operations that have been undone.
        /// </summary>
        IEnumerable<IUndoOperation> OperationsUndone { get; }

        /// <summary>
        /// Indicates if there are any undone operations.
        /// </summary>
        bool HasUndoneOperations { get; }

        /// <summary>
        /// Event fired when an operation is done.
        /// </summary>
        event EventHandler<IUndoOperation> OperationDone;

        /// <summary>
        /// Event fired when an operation is undone.
        /// </summary>
        event EventHandler<IUndoOperation> OperationUndone;

        /// <summary>
        /// Registers and invokes a new operation on the system.
        /// </summary>
        /// <param name="operation"></param>
        void Do(IUndoOperation operation);

        /// <summary>
        /// Undoes a previously registered operation.
        /// </summary>
        void Undo();

        /// <summary>
        /// Redoes a previously undone operation.
        /// </summary>
        void Redo();
    }

    /// <summary>
    /// Exception used for undo/redo operations.
    /// </summary>
    public class UndoException : Exception
    {
        public UndoException() { }
    }

    public class UndoManager : IUndoManager
    {
        private readonly Queue<IUndoOperation> operations;
        private readonly Queue<IUndoOperation> operationsUndone;

        public event EventHandler<IUndoOperation> OperationDone = delegate {};
        public event EventHandler<IUndoOperation> OperationUndone = delegate {};

        public UndoManager()
        {
            operations = new Queue<IUndoOperation>();
            operationsUndone = new Queue<IUndoOperation>();
        }

        public IEnumerable<IUndoOperation> Operations
        {
            get { return operations; }
        }

        public IEnumerable<IUndoOperation> OperationsUndone
        {
            get { return operationsUndone; }
        }

        public bool HasUndoneOperations
        {
            get { return operationsUndone.Count > 0; }
        }

        public void Do(IUndoOperation operation)
        {
            Do(operation, clearUndone: true);
        }

        private void Do(IUndoOperation operation, bool clearUndone)
        {
            operation.Do();
            operations.Enqueue(operation);

            if(clearUndone)
                operationsUndone.Clear();

            OperationDone(this, operation);
        }

        public void Undo()
        {
            if(operations.Count == 0)
                throw new UndoException();

            var operation = operations.Peek();
            operation.Undo();

            operations.Dequeue();
            operationsUndone.Enqueue(operation);

            OperationUndone(this, operation);
        }

        public void Redo()
        {
            if (operationsUndone.Count == 0)
                throw new UndoException();

            var operation = operationsUndone.Peek();
            Do(operation, clearUndone: false);
            operationsUndone.Dequeue();

            OperationDone(this, operation);
        }
    }
}
