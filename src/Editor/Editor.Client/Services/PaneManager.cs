
using System.Threading.Tasks;
using Flood.Remoting.Metadata;
using GUI.Controls;

namespace Editor.Services
{
    [DataObject(1)]
    public class Pane
    {
        //[Id(1)] 
        public string Title;

        public virtual Control CreateControl(Control parent)
        {
            return new Button(parent);
        }
    }

    //[GlobalService(1)]
    public interface IPaneManager
    {
        [Id(1)]
        Task AddPane(Pane pane);
    }

    // Implementation of global service IPaneManager
    public class PaneManager : IPaneManager
    {
        public async Task AddPane(Pane pane)
        {
            
        }
    }
}
