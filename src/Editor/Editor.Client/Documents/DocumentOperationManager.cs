using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Flood.Editor
{
    public class UndoManager<TDoc> where TDoc : Document<TDoc>
    {
        private TDoc document;

        private Queue<UndoOperation<TDoc>> operations;
        private Queue<UndoOperation<TDoc>> operationsUndone;

        public UndoManager(Document<TDoc> document)
        {
            this.document = (TDoc)document;
            operations = new Queue<UndoOperation<TDoc>>();
            operationsUndone = new Queue<UndoOperation<TDoc>>();
        }

        public void Do(UndoOperation<TDoc> operation)
        {
            Do(operation, true);
        }

        private void Do(UndoOperation<TDoc> operation, bool clearUndone)
        {
            operation.Do(document);
            operations.Enqueue(operation);
            if(clearUndone)
                operationsUndone.Clear();
        }

        public void Undo()
        {
            if(operations.Count == 0)
                return;

            var operation = operations.Peek();
            operation.Undo(document);
            operations.Dequeue();
            operationsUndone.Enqueue(operation);
        }

        public void Redo()
        {
            if (operationsUndone.Count == 0)
                return;

            var operation = operationsUndone.Peek();
            Do(operation, false);
            operationsUndone.Dequeue();
        }

    }
}
