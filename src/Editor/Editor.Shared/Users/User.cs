using System;
using System.Collections.Generic;
using System.Threading.Tasks;

namespace Flood.Editor.Shared
{
    public class User: IUser
    {
        /// <summary>
        /// User name.
        /// </summary>
        //public Task<string> Name { get; private set; }

        /// <summary>
        /// User id.
        /// </summary>
        //public Task<Guid> Id { get; private set; }

        /// <summary>
        /// List of projects user is member of and permission level of the user.
        /// </summary>
        //public Task<Dictionary<Guid, ProjectUser>> ProjectList { get; private set; }

        /// <summary>
        /// Validate user 
        /// </summary>
        public async Task<bool> Auth()
        {
            throw new NotImplementedException();
        }

        /// <summary>
        /// Get user validation status 
        /// </summary>
        public async Task<bool> GetAuthStatus()
        {
            throw new NotImplementedException();
        }

        /// <summary>
        /// Get user info
        /// </summary>
        public async Task<UserInfo> GetInfo()
        {
            throw new NotImplementedException();
        }

        /// <summary>
        /// Add project to user list
        /// </summary>
        public async Task AddProject(Guid projectId, ProjectUser user)
        {
            //ProjectList[projectId] = user;
        }

        /// <summary>
        /// Remove project from user list
        /// </summary>
        /// <returns>True if the project was removed, false otherwise.</returns>
        public async Task<bool> RemoveProject(Guid projectId)
        {
            //return ProjectList.Remove(projectId);
            return false;
        }

        public User(Guid id,  string name)
        {
            //Id = id;
            //Name = name;
        }
    }
}
