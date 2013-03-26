using Mono.Addins;

namespace Flood.Editor.Client.Commands
{
    [TypeExtensionPoint]
    public interface ICommand
    {
        void Run ();
    }
}
