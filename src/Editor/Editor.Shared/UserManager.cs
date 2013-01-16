using System;
using Flood.RPC.Metadata;

namespace Flood.Editor
{
    [Message]
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

    public enum UserMessageIds
    {
        Auth,
        AuthStatus,
        ChangeName,
        Join,
        Leave,
        NotifyName,
        GetUserInfo,
    }

    public struct UserAuth
    {
        public string Name;
        public UserAuthType AuthType;
    }

    public struct UserAuthStatus
    {
        public bool Status;
    }

    public struct UserJoin
    {
        public UserId User;
        public string Name;
    }

    public struct UserLeave
    {
        
    }

    [Message]
    public struct UserInfo
    {
        
    }

    [Service]
    public interface IUser
    {
        bool Auth();
        bool GetAuthStatus();
        UserInfo GetInfo();
    }

    [Service]
    public interface IUserManager
    {
        bool Auth([Id(0)] UserId user);

        /// <summary>
        /// Event fired when a new user is created.
        /// </summary>
        event EventHandler<IUser> UserCreated;
    }
}
