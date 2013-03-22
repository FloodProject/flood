using Flood.GUI.Controls;
using Mono.Addins;

namespace Editor.Client.GUI
{
    /// <summary>
    /// 
    /// </summary>
    [TypeExtensionPoint]
    public class Pane 
    {
        public string Title = "Pane";

        public virtual Control CreateControl(Control parent)
        {
            return new Button(parent);
        }
    }

    public class PaneNode: ExtensionNode
    {
        [NodeAttribute]
        string title;

        [NodeAttribute]
        string paneType;

        public string Title { get { return title; } }
        public string PaneType { get { return paneType; } }

        private Pane pane;

        public Pane GetPane()
        {
            if(pane == null)
                pane = (Pane) Addin.CreateInstance(PaneType);

            return pane;
        }
    }
}
