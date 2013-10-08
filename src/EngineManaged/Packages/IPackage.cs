
using Flood.Remoting;

namespace Flood.Packages
{
    public interface IPackage
    {
        void OnLoad(MessageProcessor messageProcessor);

        void Update();
    }
}
