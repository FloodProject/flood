using Flood.RPC.Metadata;

namespace Flood.Editor.Shared
{
    [Message]
    public class Owner : ProjectUser
    {
        public Owner(User sysUser, Project proj)
            : base(sysUser, proj)
        {
            UserPermissions = new Permissions();
            UserPermissions.OwnerPermissions();
        }  
    }
}
