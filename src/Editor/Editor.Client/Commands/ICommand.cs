using Mono.Addins;

namespace Editor.Client.Commands
{
    [TypeExtensionPoint]
    public interface ICommand
    {
        void Run ();
    }
}
