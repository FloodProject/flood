using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Editor.Client.Commands;
using Mono.Addins;

namespace Editor.Client.Extensions
{
    public class MenuItemNode: ExtensionNode
    {
        [NodeAttribute]
        string icon;

        [NodeAttribute]
        string label;
        
        [NodeAttribute]
        string commandType;

        public string Icon { get { return icon; } }
        public string Label { get { return label; } }
        public string CommandType { get { return commandType; } }

        public void RunCommand()
        {
            var command = (ICommand) Addin.CreateInstance (commandType);
            command.Run ();
        }
    }

    public class SubmenuNode: ExtensionNode
    {
        [NodeAttribute]
        string icon;

        [NodeAttribute]
        string label;

        public string Icon { get { return icon; } }
        public string Label { get { return label; } }
    }

    public class MenuSeparatorNode: ExtensionNode
    {
    }
}
