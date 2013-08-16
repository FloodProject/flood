using System.Threading.Tasks;
using Flood.Editor.Shared;
using Flood.RPC.Metadata;
using System;
using System.Collections.Generic;

namespace Flood.Editor
{
    [DataObject("B68611E4-9E95-4A83-9EB6-547375F7D67C")]
    public struct UserId
    {
        [Id(0)]
        public long Id;
    }

    public enum UserAuthType
    {
        None,
        Password,
        Certificate
    }

 /* 
  * public enum UserMessageIds
    {
        Auth,
        AuthStatus,
        ChangeName,
        Join,
        Leave,
        NotifyName,
        GetUserInfo,
    }
    */

    public struct Credential
    {
        public UserAuthType Type;
        public byte[] Cred;
    }

    public struct UserAuth
    {
        public string Name;
        public UserAuthType AuthType;
        public Credential Cred;
    }

    public struct UserAuthStatus
    {
        public bool Status;
    }

/*
    public struct UserJoin
    {
        public UserId ProjectUser;
        public string Name;
    }

    public struct UserLeave
    {
        
    }
 */

    [DataObject("3E35A72D-888E-4152-BA38-6326562FC3AD")]
    public struct UserInfo
    {
        [Id(0)]
        public string Name;

        [Id(1)]
        public byte[] Id;

        [Id(2)]
        public bool Auth;

        [Id(3)]
        public Dictionary<Guid, ProjectUser> ProjectList;
    }

    [Service]
    public interface IUser
    {
        /// <summary>
        /// User name.
        /// </summary>
        //Task<string> Name { get; }

        /// <summary>
        /// User id.
        /// </summary>
        //Task<Guid> Id { get; }

        /// <summary>
        /// List of projects user is member of and permission level of the user.
        /// </summary>
        //Task<Dictionary<Guid, ProjectUser>> ProjectList { get; }

        /// <summary>
        /// Validate user 
        /// </summary>
        [Id(1)]
        Task<bool> Auth();

        /// <summary>
        /// Get user validation status 
        /// </summary>
        [Id(2)]
        Task<bool> GetAuthStatus();

        /// <summary>
        /// Get user info
        /// </summary>
        [Id(3)]
        Task<UserInfo> GetInfo();

        /// <summary>
        /// Add project to user list
        /// </summary>
        /// <remarks/> can be  used to change user permission level(Owner, member ...) within existing project
        [Id(4)]
        Task AddProject([Id(0)]Guid projectId, [Id(1)]ProjectUser user);

        /// <summary>
        /// Remove project from user list
        /// </summary>
        /// <returns>True if the project was removed, false otherwise.</returns>
        [Id(5)]
        Task<bool> RemoveProject([Id(0)]Guid projectId);
    }

    [Service]
    public interface IUserManager
    {
        [Id(1)]
        Task<bool> Auth([Id(0)] UserId user);

        /// <summary>
        /// Event fired when a new user is created.
        /// </summary>
        //event EventHandler<IUser> UserCreated;
    }
}
