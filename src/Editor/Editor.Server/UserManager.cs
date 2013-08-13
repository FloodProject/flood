using System;
using System.Collections.Generic;
using System.Threading.Tasks;

namespace Flood.Editor.Server
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

        public async Task<bool> Auth(UserId user)
        {
            throw new System.NotImplementedException();
        }

        public event EventHandler<IUser> UserCreated;
    }
}