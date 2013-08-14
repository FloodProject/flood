using Flood.RPC.Metadata;

namespace Flood.Editor.Shared
{
    [Message("FDEBE89A-3011-4C5C-977B-F4373D62DF52")]
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
