using System;
using System.Collections.Generic;
using Flood.RPC.Metadata;

namespace Flood.Editor.Shared
{
    [Message]
    public class User: IUser
    {
        /// <summary>
        /// Validate user 
        /// </summary>
        public bool Auth()
        {
            throw new NotImplementedException();
        }

        /// <summary>
        /// Get user validation status 
        /// </summary>
        public bool GetAuthStatus()
        {
            throw new NotImplementedException();
        }

        /// <summary>
        /// Get user info
        /// </summary>
        public UserInfo GetInfo()
        {
            throw new NotImplementedException();
        }

        /// <summary>
        /// User name.
        /// </summary>
        [Id(0)]
        public string Name { get; private set; }

        /// <summary>
        /// User id.
        /// </summary>
        [Id(1)]
        public Guid Id { get; private set; }

        /// <summary>
        /// List of projects user is member of and permission level of the user.
        /// </summary>
        [Id(2)]
        public Dictionary<Guid, ProjectUser> ProjectList { get; private set; }

        /// <summary>
        /// Add project to user list
        /// </summary>
        /// <remarks/> can be  used to change user permission level(Owner, member ...) within existing project
        public void AddProject(Guid projectId, ProjectUser user)
        {
            ProjectList[projectId] = user;
        }

        /// <summary>
        /// Remove project from user list
        /// </summary>
        /// <returns>True if the project was removed, false otherwise.</returns>
        public bool RemoveProject(Guid projectId)
        {
            return ProjectList.Remove(projectId);
        }

        public User(Guid id,  string name)
        {
            Id = id;
            Name = name;
        }
    }
}
