using Flood.RPC.Metadata;

namespace Flood.Editor.Shared
{
    [Message]
    public class Member : ProjectUser
    {
        public Member(User sysUser, Project proj): base(sysUser, proj)
        {
            UserPermissions = new Permissions();
            UserPermissions.MemberPermissions();
        }  
    }
}
