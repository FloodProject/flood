using Flood.GUI;
using Flood.GUI.Controls;

namespace Editor.Client.GUI
{
    /// <summary>
    /// Display multiple PaneGroups side by side in an axis.
    /// </summary>
    public class Container : MultiSplitter
    {

        public Container(Control parent) : base(parent)
        {
            if (PaneManager.FocusedContainer==null)
                PaneManager.FocusedContainer = this;
        }

        protected override void OnChildAdded(Control child)
        {
            var newPaneGroup = child as PaneGroup;
            if (newPaneGroup == null)
                return;
            
           newPaneGroup.TabRemoved += control =>
                                           {
                                               if (newPaneGroup.TabCount == 0)
                                                   RemoveEmptyPaneGroups();
                                           };
        }

        protected override void OnKeyboardFocus()
        {
            base.OnKeyboardFocus();
            PaneManager.FocusedContainer = this;
        }

        private void RemoveEmptyPaneGroups()
        {
            for (var x = Panels.Count-1; x >= 0 ; x--)
            {
                var paneGroup = Panels[x] as PaneGroup;
                if (paneGroup == null || paneGroup.TabCount != 0) 
                    continue;

                RemoveChild(paneGroup, true);
            }
            ReplaceContainerByPaneGroup(this);
        }

        private static void ReplaceContainerByPaneGroup(Container childContainer)
        {
            var parentContainer = childContainer.Parent as Container;
            if (parentContainer == null)
                return;

            if (childContainer.Panels.Count != 1) 
                return;

            parentContainer.ReplacePanel(childContainer,childContainer.Panels[0],true);
            MergeSubContainers(parentContainer);
        }

        private static void MergeSubContainers(Container parentContainer)
        {
            var parentContainerSize = (parentContainer.IsHorizontal)? parentContainer.Width : parentContainer.Height;

            for (var parentPIndex = parentContainer.Panels.Count - 1; parentPIndex >= 0  ; parentPIndex--)
            {
                var childContainer = parentContainer.Panels[parentPIndex] as Container;
                if (childContainer == null || 
                    childContainer.IsHorizontal != parentContainer.IsHorizontal)
                    continue;

                for (var childPIndex = childContainer.Panels.Count - 1; childPIndex > 0  ; childPIndex--)
                {
                    var childPanel = childContainer.Panels[childPIndex];
                    parentContainer.InsertPanel(childPanel,childContainer,false);
                    
                    var childPanelSize = (childContainer.IsHorizontal)? childPanel.Width : childPanel.Height;
                    
                    var rightRPos = 1.0f;
                    if(parentContainer.Splitters.Count > parentPIndex + 1)
                        rightRPos = parentContainer.Splitters[parentPIndex+1].RelativePosition;

                    var right = rightRPos*parentContainerSize;
                    var leftSplitter = parentContainer.Splitters[parentPIndex];
                    leftSplitter.RelativePosition = (right - childPanelSize)/parentContainerSize;
                }

                ReplaceContainerByPaneGroup(childContainer);
            }
            parentContainer.Invalidate();
        }
    }
}
