using Editor.Client.GUI;
using Editor.Client.GUI.Controls;
using Flood.GUI;
using Flood.GUI.Controls;
using Flood.GUI.DragDrop;

namespace Editor.Client
{
    public static class PaneManager
    {
        private static DockHelper dockHelper;

        internal static PaneGroup FocusedPaneGroup { get; set; }
        internal static Container FocusedContainer { get; set; }

        public static void AddPane(Pane pane)
        {
            if(FocusedPaneGroup == null)
            {
                FocusedPaneGroup = new PaneGroup(FocusedContainer);
                FocusedContainer.InsertPanel(FocusedPaneGroup);
            }
            FocusedPaneGroup.AddPane(pane);
        }

        internal static void ShowDockHelper(PaneGroup paneGroup)
        {
            if (dockHelper == null || dockHelper.Disposed)
            {
                dockHelper = new DockHelper(paneGroup);
            }
            else
            {
                dockHelper.Parent = paneGroup;
                dockHelper.Invalidate();
            }
            dockHelper.IsHidden = false;

            if (DragAndDrop.SourceControl != null && //TabButton
                DragAndDrop.SourceControl.Parent != null && //TabStrip
                DragAndDrop.SourceControl.Parent.Parent is PaneGroup)
            {
                var sourcePaneGroup = (PaneGroup) DragAndDrop.SourceControl.Parent.Parent;
                if(sourcePaneGroup.TabCount == 1)
                {
                    var container = GetParentContainer(sourcePaneGroup);
                    container.RemovePanel(sourcePaneGroup,false);
                }
            }
        }

        internal static void HideDockHelper()
        {
            dockHelper.IsHidden = true;
        }

        private static Container GetParentContainer(Control control)
        {
            var container = control as Container;
            if(container != null)
                return container;

            return GetParentContainer(control.Parent);
        }

        /// <summary>
        /// Move pane from the current PaneGroup to the targetPaneGroup. 
        /// </summary>
        /// <param name="pane">Pane to be moved</param>
        /// <param name="targetPaneGroup"></param>
        internal static void MovePane(TabButton pane, PaneGroup targetPaneGroup)
        {
            targetPaneGroup.AddPage(pane);
        }

        /// <summary>
        /// Move pane to a container relative to targetPaneGroup. 
        /// </summary>
        /// <param name="pane">Pane to be moved</param>
        /// <param name="targetPaneGroup"> </param>
        /// <param name="depth">Refer a parent container of targetPaneGroup, each depth is one parent container away. 
        /// Depth 0 add a PaneContainer in the same container if the container is horizontal, if not
        /// adds the movedPane to a new child vertical container</param>
        /// <param name="moveRigth"> True to move pane to the rigth of targetPaneGroup </param>
        internal static void MovePaneHorizontally(TabButton pane, PaneGroup targetPaneGroup, uint depth, bool moveRigth)
        {
            var container = GetParentContainer(targetPaneGroup);
            if (!container.IsHorizontal)
            {
                var childContainer = new Container(container);
                childContainer.IsHorizontal = true;
                container.ReplacePanel(targetPaneGroup,childContainer,false);
                childContainer.InsertPanel(targetPaneGroup);
                container = childContainer;
            }
            var paneGroup = new PaneGroup(container);
            container.InsertPanel(paneGroup,targetPaneGroup,!moveRigth);
            paneGroup.AddPage(pane);
        }

        /// <summary>
        /// Move pane to a container relative to targetPaneGroup. 
        /// </summary>
        /// <param name="pane">Pane to be moved</param>
        /// <param name="targetPaneGroup"> </param>
        /// <param name="depth">Refer a parent container of targetPaneGroup, each depth is one parent container away. 
        /// Depth 0 add a PaneContainer in the same container if the container is vertical, if not
        /// adds the pane to a new child horizontal container</param>
        /// <param name="moveUp"> True to move pane above targetPaneGroup </param>
        internal static void MovePaneVertically(TabButton pane, PaneGroup targetPaneGroup, uint depth, bool moveUp)
        {
            var container = GetParentContainer(targetPaneGroup);
            if (container.IsHorizontal)
            {
                var childContainer = new Container(container);
                childContainer.IsHorizontal = false;
                container.ReplacePanel(targetPaneGroup,childContainer,false);
                childContainer.InsertPanel(targetPaneGroup);
                container = childContainer;
            }
            var paneGroup = new PaneGroup(container);
            container.InsertPanel(paneGroup,targetPaneGroup,moveUp);
            paneGroup.AddPage(pane);
        }
    }
}
