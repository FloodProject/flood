using Gwen.Control;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace FlushEditor.GUI.Tools
{
    class SideBar : EditorTool, DockableTool
    {
        public void InitControl(Gwen.Control.Base parent)
        {
           /* var ptree = new Gwen.Control.PropertyTree(parent);

            ptree.Dock = Gwen.Pos.Left;
            ptree.Width = 200;


            {
                var props = ptree.Add("Item One");
                props.ValueChanged += OnChanged;

                props.Add("Middle Name");
                props.Add("Last Name");
                props.Add("Four");
                props.Add("More Items");
                props.Add("Bacon", new Gwen.Control.Property.Check(props), "1");
                props.Add("To Fill");
                props.Add("Colour", new Gwen.Control.Property.Color(props), "255 0 0");
                props.Add("Out Here");
            }

            ptree.ExpandAll();*/
        }

        void OnChanged(Base control)
        {
            //PropertyRow row = control as PropertyRow;
            //Console.WriteLine(String.Format("Property changed: {0}", row.Value));
        }
    }
}
