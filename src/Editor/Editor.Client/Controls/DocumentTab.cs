using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Flood.GUI.Controls;
using Flood.GUI;

namespace Flood.Editor.Controls
{
    public class DocumentTab
    {
        TabControl tabControl;

        Dictionary<int,TabButton> tabButtons;

        public event Action<int> DocumentSelected;

        public DocumentTab(Control parent)
        {
            tabControl = new TabControl(parent)
                {
                    Dock = Pos.Fill
                };

            tabButtons = new Dictionary<int, TabButton>();

            tabControl.TabSelected += OnTabSelected;
        }

        private void OnTabSelected(Control control)
        {
            if (DocumentSelected != null)
                DocumentSelected.Invoke(tabButtons.Single(kvp => kvp.Value == control).Key);
        }

        public void AddTab(int id, string name, Action<Control> createControl)
        {
            tabControl.TabSelected -= OnTabSelected;
            var button = tabControl.AddPage(name);
            tabControl.TabSelected += OnTabSelected;

            tabButtons.Add(id, button);

            OnTabSelected(button);

            var page = button.Page;

            createControl.Invoke(page);

        }

        public void RemoveTab(int id)
        {
            if (tabButtons.ContainsKey(id))
            {
                tabControl.RemovePage(tabButtons[id]);// RemoveChild(tabButtons[id], false);
                tabControl.Redraw();
                tabButtons.Remove(id);
            }
        }

        public void SelectTab(int id)
        {
            if (tabButtons.ContainsKey(id))
            { 
                tabControl.TabSelected -= OnTabSelected;
                tabButtons[id].Press();
                tabControl.TabSelected += OnTabSelected;
            }
        }

        public int CurrentTab {
            get {
                if (tabControl.CurrentButton == null || tabButtons.Count == 0)
                    return 0;

                return tabButtons.Single(kvp => kvp.Value == tabControl.CurrentButton).Key; 
            }
        }
    }
}
