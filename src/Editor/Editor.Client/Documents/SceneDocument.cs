using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Flood.Editor.GUI.Documents
{
    public class SceneDocument : Document
    {
        public SceneDocument(string path, int id)
            : base(path, id)
        {

        }

        public override void InitGUI(Gwen.Control.Base rootGUI)
        {
            var label = new Gwen.Control.Label(rootGUI);
            label.SetText("YEAH");
            label.Height = 100;
        }
    }
}
