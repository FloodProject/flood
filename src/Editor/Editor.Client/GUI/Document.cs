using Flood.Editor.GUI.Tools;
using System;
using System.Collections.Generic;
using System.ComponentModel.Composition;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Flood.Editor.GUI
{
    public abstract class Document : Document<Document> {

        public Document(string path, int id) : base(path, id)
        {

        }
    
    }

    public abstract class  Document<TDoc>
        where TDoc : Document<TDoc>
    {
        public UndoManager<TDoc> UndoManager { get; private set; }

        public string Path { get; private set; }

        public int Id { get; private set; }

        public Document(string path, int id){
            Path = path;
            Id = id;
            UndoManager = new UndoManager<TDoc>(this);
        }

        public abstract void InitGUI(Gwen.Control.Base rootGUI);

        /*public abstract void FromBuffer(byte[] buffer);
        public abstract byte[] ToBuffer();

        public static TDoc Create(string path) 
        {
            //byte[] buffer = null;
            return null; // FromBuffer(buffer);
        }

        public void Save()
        {
            byte[] buffer = ToBuffer();

        }*/
        
    }
}
