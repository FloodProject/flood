using Flood.Remoting;

namespace Flood.Modules
{
    public interface IModule
    {
        void OnLoad(ServiceManager serviceManager);

        void Update();
    }
}
