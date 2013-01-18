using System.Linq;
using Flood.Editor.Documents;
using System;
using System.Collections.Generic;
using System.ComponentModel.Composition;

namespace Flood.Editor
{
    public class DocumentManager : IPartImportsSatisfiedNotification
    {
        int nextDocumentId = 1;
        readonly Dictionary<int,Document> documents;
        readonly LinkedList<Document> documentSelectionHistory;

        public event Action<Document> DocumentSelected;
        public event Action<Document> DocumentAdded;
        public event Action<Document> DocumentRemoved;

        [Import]
        EditorWindow editorWindow;

        public Document Current
        {
            get
            {
                return documentSelectionHistory.Count == 0 ?
                    null : documentSelectionHistory.First.Value;
            }
            set
            {
                if (documentSelectionHistory.Count != 0
                    && value == documentSelectionHistory.First.Value)
                    return;

                if (documentSelectionHistory.Contains(value))
                    documentSelectionHistory.Remove(value);

                documentSelectionHistory.AddFirst(value);

                if (DocumentSelected != null)
                    DocumentSelected.Invoke(value);
            }
        }

        public DocumentManager()
        {
            documents = new Dictionary<int, Document>();
            documentSelectionHistory = new LinkedList<Document>();
        }

        public void OnImportsSatisfied()
        {
            editorWindow.DocumentTab.DocumentSelected += delegate(int id)
            {
                Document doc = null;
                if (documents.TryGetValue(id, out doc))
                    Current = doc;
            };

            //DocumentSelected += d => editorWindow.DocumentTab.SelectTab(d.Id);
        }

        public bool FindDocument(string path, out Document document)
        {
            document = null;
            foreach (var doc in documents.Values.Where(doc => doc.Path == path))
            {
                document = doc;
                return true;
            }
            return false;
        }

        public void Open(string path)
        {
            var id = nextDocumentId++;
            //Document doc = new SceneDocument(path, id);
            //editorWindow.DocumentTab.AddTab(id, "Doc" + id, doc.InitGUI);
            //Add(id, doc, true);
        }

        public void Close(int id)
        {
            Remove(id);
            editorWindow.DocumentTab.RemoveTab(id);
        }

        private void Add(int id, Document document, bool select = true)
        {
            documents.Add(id, document);
            if(select)
                Current = document;

            if (DocumentAdded != null)
                DocumentAdded.Invoke(document);
        }

        private void Remove(int id)
        {
            if (!documents.ContainsKey(id))
                return;

            var document = documents[id];

            //var dirtySelection = id == Current.Id;

            documents.Remove(id);
            
            if(documentSelectionHistory.Contains(document))
                documentSelectionHistory.Remove(document);

            if (DocumentRemoved != null)
                DocumentRemoved.Invoke(document);

            //if (dirtySelection && Current != null)
            //   DocumentSelected.Invoke(Current);
        }

        public void SaveCurrent()
        {
            Console.WriteLine("SaveCurrent");
        }
    }
}
