using System;
using Flood.GUI;
using Flood.GUI.Controls;
using System.ComponentModel.Composition;

namespace Flood.Editor.Controls
{
    public class ProjectNode
    {
        /// <summary>
        /// GUI tree node.
        /// </summary>
        public TreeNode Node { get; private set; }

        protected internal ProjectNode(Control parent, string name)
        {
            Node = new TreeNode(parent)
            {
                Text = name,
                Dock = Pos.Top
            };
        }
    }

    /// <summary>
    /// Represents the pane with the projects.
    /// </summary>
    public class ProjectPane : Pane
    {

        /// <summary>
        /// GUI tree node.
        /// </summary>
        public TreeControl Tree { get; private set; }

        public override void InitControl(Control parent)
        {
            Tree = new TreeControl(parent)
            {
                Dock = Pos.Left,
                Width = 150
            };

            CreateProjectNode("test");
        }

        public ProjectNode CreateProjectNode(string name)
        {
            var node = new ProjectNode(Tree, name);

            return node;
        }

        public void RemoveProjectNode(string name)
        {
            var child = Tree.FindChildByName(name);

            if (child != null)
                Tree.RemoveChild(child, dispose: true);
        }


        void OnProjectAdded(object sender, Project project)
        {
            CreateProjectNode(project.Name);
        }

        void OnProjectRemoved(object sender, Project project)
        {
            RemoveProjectNode(project.Name);
        }
    }
}
