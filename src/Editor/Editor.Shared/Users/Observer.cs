using Flood.RPC.Metadata;

namespace Flood.Editor.Shared
{
    [Message("96FCE219-509A-453A-A291-36FEB82971AD")]
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
