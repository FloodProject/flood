using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Flood.RPC.Metadata;

namespace Flood.Editor.Shared
{
    public enum UserType
    {
        Owner,
        Member,
        Observer
    }

    [Message]
    public abstract class ProjectUser
    {

        /// <summary>
        /// User permission.   
        /// </summary>
        [Id(0)]
        public Permissions UserPermissions { get; set; }

        /// <summary>
        /// User system identity.   
        /// </summary>
        [Id(1)]
        public User SystemUser { get; private set; }

        /// <summary>
        /// Project user is assigned to.   
        /// </summary>
        [Id(2)]
        public Project Proj { get; private set; }

        /// <summary>
        /// User id   
        /// </summary>
        [Id(3)]
        public Guid Id
        {
            get { return SystemUser.Id; }
        }

        protected ProjectUser(User sysUser, Project proj)
        {
            SystemUser = sysUser;
            Proj = proj;
        }
    }
}
