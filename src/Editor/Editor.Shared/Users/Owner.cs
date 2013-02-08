using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
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
