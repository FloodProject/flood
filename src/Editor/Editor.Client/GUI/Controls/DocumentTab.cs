using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Gwen.Control;
using Gwen;

namespace Flood.Editor.GUI.Controls
{
    public class DocumentTab
    {
        Gwen.Control.TabControl tabControl;

        Dictionary<int,TabButton> tabButtons;

        public event Action<int> DocumentSelected;


        public DocumentTab(Gwen.Control.Base parent)
        {
            tabControl = new Gwen.Control.TabControl(parent);
            tabControl.Dock = Gwen.Pos.Fill;

            tabButtons = new Dictionary<int, TabButton>();

            tabControl.TabSelected += OnTabSelected;
        }

        private void OnTabSelected(Gwen.Control.Base control)
        {
            if (DocumentSelected != null)
                DocumentSelected.Invoke(tabButtons.Single(kvp => kvp.Value == control).Key);
        }

        public void AddTab(int id, string name, Action<Gwen.Control.Base> createControl)
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
