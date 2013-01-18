using Flood.Editor;
using NUnit.Framework;
using NSubstitute;

namespace Editor.Tests
{
    [TestFixture]
    public class UndoTests
    {
        IUndoManager undo;
        IUndoOperation operation;
        bool eventFired;

        [SetUp]
        public void Init()
        {
            undo = new UndoManager();
            operation = Substitute.For<IUndoOperation>();
            eventFired = false;
        }

        [Test]
        public void EmptyUndo()
        {
            Assert.Throws<UndoException>(() => undo.Undo());
        }

        [Test]
        public void EmptyRedo()
        {
            Assert.Throws<UndoException>(() => undo.Redo());
        }

        [Test]
        public void DoOperation()
        {
            undo.OperationDone += (sender, undoOperation) => eventFired = true;

            undo.Do(operation);
            operation.Received().Do();

            Assert.IsTrue(eventFired);
            Assert.That(undo.Operations, Has.Exactly(1).EqualTo(operation));
            Assert.That(undo.OperationsUndone, Is.Empty);
        }

        [Test]
        public void UndoOperation()
        {
            undo.OperationUndone += (sender, undoOperation) => eventFired = true;

            undo.Do(operation);
            undo.Undo();
            operation.Received().Undo();

            Assert.IsTrue(eventFired);
            Assert.That(undo.Operations, Is.Empty);
            Assert.That(undo.OperationsUndone, Has.Exactly(1).EqualTo(operation));
        }

        [Test]
        public void RedoOperation()
        {
            undo.OperationDone += (sender, undoOperation) => eventFired = true;

            undo.Do(operation);
            undo.Undo();
            undo.Redo();

            Assert.IsTrue(eventFired);
            Assert.That(undo.Operations, Has.Exactly(1).EqualTo(operation));
            Assert.That(undo.OperationsUndone, Is.Empty);
        }

        [Test]
        public void UndoneOperations()
        {
            Assert.IsFalse(undo.HasUndoneOperations);

            undo.Do(operation);
            Assert.IsFalse(undo.HasUndoneOperations);

            undo.Undo();
            Assert.IsTrue(undo.HasUndoneOperations);

            undo.Redo();
            Assert.IsFalse(undo.HasUndoneOperations);
        }
    }
}