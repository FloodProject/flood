
using System.Threading.Tasks;
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
        public async Task AddPane(Pane pane)
        {
            
        }
    }
}
