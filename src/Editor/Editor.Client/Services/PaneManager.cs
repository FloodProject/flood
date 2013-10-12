
using System.Collections.Generic;
using System.Threading.Tasks;
using Flood.Editor.Client.GUI;
using Flood.GUI;
using Flood.Remoting.Metadata;
using Flood.GUI.Controls;

namespace Editor.Services
{
    [DataObject(1)]
    public class Pane
    {
        [Id(1)] 
        public string Title;

        [Id(2)]
        public Control Control;
    }

    [GlobalService(1)]
    public interface IPaneManager
    {
        [Id(1)]
        Task AddPane([Id(1)]Pane pane);
    }

    // Implementation of global service IPaneManager
    public class PaneManager : IPaneManager
    {
        private Container container;
        private LinkedList<PaneGroup> FocusedGroup;

        public PaneManager(Canvas canvas)
        {
            container = new Container(canvas);
            container.Dock = Pos.Fill;

            FocusedGroup = new LinkedList<PaneGroup>();
        }

        public PaneGroup Focus
        {
            get { return FocusedGroup.First.Value; }
            set
            {
                FocusedGroup.Remove(value);
                FocusedGroup.AddFirst(value);
            }
        }

        public void RemovePane(PaneGroup paneGroup)
        {
            FocusedGroup.Remove(paneGroup);
        }

        public async Task AddPane(Pane pane)
        {
            if (FocusedGroup.Count == 0)
            {
                var paneGroup = new PaneGroup(container, this);
                container.InsertPanel(paneGroup);
                Focus = paneGroup;
            }

            Focus.AddPane(pane);
        }
    }
}
