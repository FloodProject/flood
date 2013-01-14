using Flood.Editor.GUI.Documents;
using System;
using System.Collections.Generic;
using System.ComponentModel.Composition;

namespace Flood.Editor.GUI
{
    //TODO move selection history to gwen control
    class DocumentManager : IPartImportsSatisfiedNotification
    {
        int documentIdCount = 1;
        Dictionary<int,Document> documents;
        LinkedList<Document> documentSelectionHistory;

        public event Action<Document> DocumentSelected;
        public event Action<Document> DocumentAdded;
        public event Action<Document> DocumentRemoved;

        [Import]
        EditorGUI editorGUI;

        public Document Current
        {
            get
            {
                if (documentSelectionHistory.Count == 0)
                    return null;

                return documentSelectionHistory.First.Value;
            }
            set
            {
                if (documentSelectionHistory.Count == 0 || value != documentSelectionHistory.First.Value)
                {
                    if (documentSelectionHistory.Contains(value))
                        documentSelectionHistory.Remove(value);

                    documentSelectionHistory.AddFirst(value);

                    if (DocumentSelected != null)
                        DocumentSelected.Invoke(value);
                }
            }
        }

        public DocumentManager()
        {
            documents = new Dictionary<int, Document>();
            documentSelectionHistory = new LinkedList<Document>();

           
        }

        public void OnImportsSatisfied()
        {
            editorGUI.DocumentTab.DocumentSelected += delegate(int id)
            {
                Document doc = null;
                if (documents.TryGetValue(id, out doc))
                    Current = doc;
            };

            DocumentSelected += delegate(Document d)
            {
                editorGUI.DocumentTab.SelectTab(d.Id);
            };
        }

        public bool FindDocument(string path, out Document document)
        {
            document = null;
            foreach (var doc in documents.Values)
            {
                if (doc.Path == path)
                {
                    document = doc;
                    return true;
                }
            }
            return false;
        }

        public void Open(string path)
        {
            var id = documentIdCount++;
            Document doc = new SceneDocument(path, id);
            editorGUI.DocumentTab.AddTab(id, "Doc" + id, doc.InitGUI);
            Add(id, doc, true);
            
        }

        public void Close(int id)
        {
            Remove(id);
            editorGUI.DocumentTab.RemoveTab(id);
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

            var dirtySelection = id == Current.Id;

            documents.Remove(id);
            
            if(documentSelectionHistory.Contains(document))
                documentSelectionHistory.Remove(document);

            if (DocumentRemoved != null)
                DocumentRemoved.Invoke(document);

            if (dirtySelection && Current != null)
               DocumentSelected.Invoke(Current);
        }


        public void SaveCurrent()
        {
            Console.WriteLine("SaveCurrent");
        }

 
        
    }
}
