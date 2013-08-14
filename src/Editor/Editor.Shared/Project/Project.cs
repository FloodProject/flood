using System;
using System.Collections.Generic;
using Flood.RPC.Metadata;

namespace Flood.Editor.Shared
{
    public enum ProjectType
    {
        CPlusPlus,
        CSharp,
        Game
    }

    [Message("BDA5FBCD-8D57-435A-B6D0-3DF52EB71FE4")]
    public class Project
    {
        /// <summary>
        /// Id of the project.
        /// </summary>
        [Id(0)]
        public Guid Id;

        /// <summary>
        /// Date the project was created. 
        /// </summary>
        [Id(1)]
        public DateTime DateCreated;

        /// <summary>
        /// Users assigned to the project.
        /// </summary>
        [Id(2)]
        public Dictionary<Guid, ProjectUser> Users;

        public ICollection<ProjectUser> GetUsers()
        {
            return Users.Values;
        }

        /// <summary>
        /// Current version of the project.
        /// </summary>
        [Id(3)]
        public string CurrentVersion;

        /// <summary>
        /// Description of the project.
        /// </summary>
        [Id(4)]
        public string Description;

        /// <summary>
        /// Type of the project.
        /// </summary>
        [Id(5)]
        public ProjectType Type;

        /// <summary>
        /// Name of the project.
        /// </summary>
        [Id(6)]
        public string Name;

        /// <summary>
        /// Add member to project.
        /// </summary>
        /// <returns>True if the user was added, false otherwise.</returns>
        public bool AddUser(ProjectUser user)
        {
            //if (Users.ContainsKey(user.Id))
            //    return false;

            //Users[user.Id] = user;
            return true;
        }

        /// <summary>
        /// Modifies project User.
        /// </summary>
        /// <returns>True if the user was modified, false otherwise.</returns>
        public bool ModifyUser(ProjectUser user)
        {
            //if (!Users.ContainsKey(user.Id))
            //    return false;

            //Users[user.Id] = user;
            return true;
        }

        /// <summary>
        /// Remove member from project.
        /// </summary>
        /// <returns>True if the user was removed, false otherwise.</returns>
        public bool RemoveUser(ProjectUser user)
        {
            //return Users.Remove(user.Id);
            return false;
        }

        public Project(ProjectType type, string name, Guid id, ProjectUser founder)
        {
            Type = type;
            Name = name;
            DateCreated = System.DateTime.Now;
            Id = id;
            Users = new Dictionary<Guid, ProjectUser>();
            //Users[founder.Id] = founder;
        }
    }
}
