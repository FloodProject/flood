using System.Collections.Generic;
using Editor.Services;
using Flood.Editor.Client.GUI.Controls;
using GUI;
using GUI.Controls;
using GUI.DragDrop;

namespace Flood.Editor.Client.GUI
{
    /// <summary>
    /// Display a group of panes that share the same boundaries.
    /// Navigation through the panes is done with tabs.
    /// </summary>
    public class PaneGroup : TabControl
    {
        private readonly Dictionary<Pane,TabButton> paneButtons = new Dictionary<Pane,TabButton>();
        private DockHelper dockHelper;

        public PaneGroup(Container parent) : base(parent)
        {
            AllowReorder = true;
        }

        public void AddPane(Pane pane)
        {
            var tabButton = AddPage(pane.Title, pane.CreateControl(this));
            paneButtons.Add(pane,tabButton);
        }

        public void RemovePane(Pane pane)
        {
            if(!paneButtons.ContainsKey(pane))
                return;

            var tabButton = paneButtons[pane];
            RemovePage(tabButton);
        }

        internal void ShowDockHelper()
        {
            if (dockHelper == null || dockHelper.Disposed)
                dockHelper = new DockHelper(this);

            dockHelper.IsHidden = false;
        }

        internal void HideDockHelper()
        {
            dockHelper.IsHidden = true;
        }

        public override bool DragAndDrop_CanAcceptPackage(Package p)
        {
             if (p.Name == "TabButtonMove")
                ShowDockHelper();
             return base.DragAndDrop_CanAcceptPackage(p);
        }

        private Container GetParentContainer(Control control)
        {
            var container = control as Container;
            return container ?? GetParentContainer(control.Parent);
        }

        /// <summary>
        /// Move pane from the current PaneGroup to the targetPaneGroup. 
        /// </summary>
        /// <param name="pane">Pane to be moved</param>
        /// <param name="targetPaneGroup"></param>
        internal void MovePane(TabButton pane, PaneGroup targetPaneGroup)
        {
            targetPaneGroup.AddPage(pane);
        }

        /// <summary>
        /// Move pane to a container relative to targetPaneGroup. 
        /// </summary>
        /// <param name="pane">Pane to be moved</param>
        /// <param name="depth">Refer a parent container of targetPaneGroup, each depth is one parent container away. 
        /// Depth 0 add a PaneContainer in the same container if the container is horizontal, if not
        /// adds the movedPane to a new child vertical container</param>
        /// <param name="moveRight"> True to move pane to the rigth of PaneGroup </param>
        internal void MovePaneHorizontally(TabButton pane, uint depth, bool moveRight)
        {
            var container = GetParentContainer(this);

            if (!container.IsHorizontal)
            {
                var childContainer = new Container(container) {IsHorizontal = true};
                container.ReplacePanel(this,childContainer,false);
                childContainer.InsertPanel(this);
                container = childContainer;
            }

            var paneGroup = new PaneGroup(container);
            paneGroup.AddPage(pane);
            container.InsertPanel(paneGroup,this,!moveRight);
            
        }

        /// <summary>
        /// Move pane to a container relative to targetPaneGroup. 
        /// </summary>
        /// <param name="pane">Pane to be moved</param>
        /// <param name="depth">Refer a parent container of targetPaneGroup, each depth is one parent container away. 
        /// Depth 0 add a PaneContainer in the same container if the container is vertical, if not
        /// adds the pane to a new child horizontal container</param>
        /// <param name="moveUp"> True to move pane above PaneGroup </param>
        internal void MovePaneVertically(TabButton pane, uint depth, bool moveUp)
        {
            var container = GetParentContainer(this);

            if (container.IsHorizontal)
            {
                var childContainer = new Container(container) {IsHorizontal = false};
                container.ReplacePanel(this,childContainer,false);
                childContainer.InsertPanel(this);
                container = childContainer;
            }

            var paneGroup = new PaneGroup(container);
            container.InsertPanel(paneGroup,this,moveUp);
            paneGroup.AddPage(pane);
        }
    }
}
