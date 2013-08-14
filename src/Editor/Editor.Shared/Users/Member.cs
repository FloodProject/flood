using Flood.RPC.Metadata;

namespace Flood.Editor.Shared
{
    [Message("51437B67-90D3-4722-9CCC-6FDEE5497A74")]
    public class Member : ProjectUser
    {
        public Member(User sysUser, Project proj): base(sysUser, proj)
        {
            UserPermissions = new Permissions();
            UserPermissions.MemberPermissions();
        }  
    }
}
