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
            var newPaneGroup = new PaneGroup(this);
            InsertPanel(newPaneGroup);
            Dock = Pos.Fill;
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

        private void RemoveEmptyPaneGroups()
        {
            for (var x = Panels.Count-1; x >= 0 ; x--)
            {
                var paneGroup = Panels[x] as PaneGroup;
                if (paneGroup != null)
                {
                    if (paneGroup.TabCount == 0)
                        RemoveChild(paneGroup, true);
                }
            }
        }
    }
}
