using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace FlushEditor.GUI.Tools
{

    [Tool(Text = "Side Bar")]
    class SideBar : DockableTool
    {
        public void InitControl(Gwen.Control.Base parent)
        {
            var tree = new Gwen.Control.TreeControl(parent);
            
            tree.Dock = Gwen.Pos.Right;
            tree.Width = 150;


            var projectNode = new Gwen.Control.TreeNode(tree);
            projectNode.Text = "Project";
            projectNode.Dock = Gwen.Pos.Top;

            var doc1 = new Gwen.Control.TreeNode(projectNode);
            doc1.Text = "Doc1";
            doc1.Dock = Gwen.Pos.Top;
        }
    }
}
