using System;
using Flood.Editor;
using NUnit.Framework;
using NSubstitute;

namespace Editor.Tests
{
    class TestDocument : Document
    {
        public TestDocument(IUndoManager undoManager, string path)
            : base(undoManager, path)
        {
        }
    }

    [TestFixture]
    public class DocumentTests
    {
        IDocument document;
        IUndoManager undoManager;
        bool eventFired;

        const string Path = "test.doc";

        [SetUp]
        public void Init()
        {
            undoManager = Substitute.For<IUndoManager>();
            document = new TestDocument(undoManager, Path);
            eventFired = false;
        }

        [Test]
        public void EventsDispose()
        {
            document.Dispose();
        }

        [Test]
        public void Document()
        {
            Assert.That(document.Path, Is.EqualTo(Path));
            Assert.That(document.UndoManager, Is.EqualTo(undoManager));
        }

        [Test]
        public void CtorException()
        {
            Assert.Throws<ArgumentNullException>(() => new TestDocument(null, string.Empty));
        }

        [Test]
        public void DocumentChanges()
        {
            undoManager.HasUndoneOperations.Returns(false);

            var hasChanges = document.HasUnsavedChanges;
            Assert.IsFalse(hasChanges);

            undoManager.HasUndoneOperations.Returns(true);
            undoManager.OperationUndone += Raise.Event<EventHandler<IUndoOperation>>(
                undoManager, null);

            hasChanges = document.HasUnsavedChanges;
            Assert.IsTrue(hasChanges);
        }
    }
}