using System.Collections.Generic;
using Flood.GUI.Controls;
using Flood.GUI.DragDrop;

namespace Flood.Editor.Client.GUI
{
    /// <summary>
    /// Display a group of panes that share the same boundaries.
    /// Navigation through the panes is done with tabs.
    /// </summary>
    public class PaneGroup : TabControl
    {
        private readonly Dictionary<Pane,TabButton> paneButtons = new Dictionary<Pane,TabButton>();

        public PaneGroup(Control parent) : base(parent)
        {
            if(PaneManager.FocusedPaneGroup==null)
                PaneManager.FocusedPaneGroup = this;

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

        protected override void OnKeyboardFocus()
        {
            base.OnKeyboardFocus();
            PaneManager.FocusedPaneGroup = this;
        }

        public override bool DragAndDrop_CanAcceptPackage(Package p)
        {
             if (p.Name == "TabButtonMove")
                PaneManager.ShowDockHelper(this);
             return base.DragAndDrop_CanAcceptPackage(p);
        }
    }
}
