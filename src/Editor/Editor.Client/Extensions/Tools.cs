using System;
using Editor.Client.Commands;
using Flood.GUI.Controls;
using Mono.Addins;

namespace Editor.Client.Extensions
{
    public abstract class ToolNode : ExtensionNode
    {
        public abstract Control CreateControl(Control parent);
    }

    public class ToolButtonNode: ToolNode
    {
        [NodeAttribute]
        string icon;
        
        [NodeAttribute]
        string commandType;

        public override Control CreateControl(Control parent)
        {
            var btn = new Button(parent);
            
            btn.ShouldDrawBackground = false;

            var iBuffer = IconManager.GetIconData(icon, IconSize.Invalid);
            btn.SetImage(iBuffer,true);
            btn.SetSize(20, 20);

            btn.Clicked += c => OnClicked();

            return btn;
        }

        void OnClicked()
        {
            ICommand command = (ICommand) Addin.CreateInstance (commandType);
            command.Run ();
        }
    }

    public class ToolSeparatorNode: ToolNode
    {

        public override Control CreateControl(Control parent)
        {
            throw new NotImplementedException();
        }

    }
}
