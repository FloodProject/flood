
using System;
using System.Collections.Generic;

namespace Flood.Editor
{
    public class UserManager : IUserManager
    {
        private List<IUser> users;

        /// <summary>
        /// Read-only list of all the users.
        /// </summary>
        public IReadOnlyList<IUser> Projects
        {
            get { return users; }
        }

        public bool Auth(UserId user)
        {
            throw new System.NotImplementedException();
        }

        public event EventHandler<IUser> UserCreated;
    }
}