using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Flood.GUI.Controls;

namespace Editor.Client.GUI
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
            if(PaneService.FocusPaneGroup == null)
                PaneService.FocusPaneGroup = this;
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
            PaneService.FocusPaneGroup = this;
        }
    }
}
