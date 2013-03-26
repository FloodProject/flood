using Flood.RPC.Metadata;

namespace Flood.Editor.Shared
{
    [Message]
    public class Observer : ProjectUser
    {
        public Observer(User sysUser, Project proj)
            : base(sysUser, proj)
        {
            UserPermissions = new Permissions();
            UserPermissions.ObserverPermissions();
        }  
    }
}
