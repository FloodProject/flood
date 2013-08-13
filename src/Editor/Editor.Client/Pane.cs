using Flood.GUI.Controls;

namespace Flood.Editor.Client.GUI
{
    /// <summary>
    /// 
    /// </summary>
    public class Pane 
    {
        public string Title = "Pane";

        public virtual Control CreateControl(Control parent)
        {
            return new Button(parent);
        }
    }
}
