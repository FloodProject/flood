using System;
using Gwen.Control;
using System.ComponentModel.Composition;

namespace Flood.Editor.Controls
{
    public class ProjectNode
    {
        /// <summary>
        /// GUI tree node.
        /// </summary>
        public TreeNode Node { get; private set; }

        protected internal ProjectNode(Base parent, string name)
        {
            Node = new TreeNode(parent)
            {
                Text = name,
                Dock = Gwen.Pos.Top
            };
        }
    }

    [Export]
    public class ProjectPane : EditorTool, DockableTool
    {
        /// <summary>
        /// GUI tree node.
        /// </summary>
        public TreeControl Tree { get; private set; }

        public void InitControl(Base parent)
        {
            Tree = new TreeControl(parent)
            {
                Dock = Gwen.Pos.Left,
                Width = 150
            };
        }

        public ProjectNode CreateProjectNode(string name)
        {
            var node = new ProjectNode(Tree, name);
            return node;
        }
    }
}
