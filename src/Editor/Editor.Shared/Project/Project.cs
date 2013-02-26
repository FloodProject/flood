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

    public interface IProject
    {

        /// <summary>
        /// Id of the project.
        /// </summary>
        Guid Id { get; }

        /// <summary>
        /// Date the project was created. 
        /// </summary>
        DateTime DateCreated { get; }

        /// <summary>
        /// List of users assigned to the project.
        /// </summary>
        ICollection<ProjectUser> GetUsers();

        /// <summary>
        /// Current version of the project.
        /// </summary>
        string CurrentVersion { get; }

        /// <summary>
        /// Description of the project.
        /// </summary>
        string Description { get; }

        /// <summary>
        /// Type of the project.
        /// </summary>
        ProjectType Type { get; }

        /// <summary>
        /// Name of the project.
        /// </summary>
        string Name { get; }

        /// <summary>
        /// Add member to project.
        /// </summary>
        /// <returns>True if the user was added, false otherwise.</returns>
        bool AddUser(ProjectUser user);

        /// <summary>
        /// Remove member from project.
        /// </summary>
        /// <returns>True if the user was removed, false otherwise.</returns>
        bool RemoveUser(ProjectUser user);

        /// <summary>
        /// Modifies project User.
        /// </summary>
        /// <returns>True if the user was modified, false otherwise.</returns>
        bool ModifyUser(ProjectUser user);


    }


    [Message]
    public class Project : IProject
    {

        /// <summary>
        /// Id of the project.
        /// </summary>
        [Id(0)]
        public Guid Id { get; private set; }

        /// <summary>
        /// Date the project was created. 
        /// </summary>
        [Id(1)]
        public DateTime DateCreated { get; private set; }

        /// <summary>
        /// Users assigned to the project.
        /// </summary>
        [Id(2)]
        public Dictionary<Guid, ProjectUser> Users { get; private set; }

        public ICollection<ProjectUser> GetUsers()
        {
            return Users.Values;
        }

        /// <summary>
        /// Current version of the project.
        /// </summary>
        [Id(3)]
        public string CurrentVersion { get; private set; }

        /// <summary>
        /// Description of the project.
        /// </summary>
        [Id(4)]
        public string Description { get; private set; }

        /// <summary>
        /// Type of the project.
        /// </summary>
        [Id(5)]
        public ProjectType Type { get; private set; }

        /// <summary>
        /// Name of the project.
        /// </summary>
        [Id(6)]
        public string Name { get; private set; }

        /// <summary>
        /// Add member to project.
        /// </summary>
        /// <returns>True if the user was added, false otherwise.</returns>
        public bool AddUser(ProjectUser user)
        {
            if (Users.ContainsKey(user.Id))
                return false;
            Users[user.Id] = user;
            return true;
        }

        /// <summary>
        /// Modifies project User.
        /// </summary>
        /// <returns>True if the user was modified, false otherwise.</returns>
        public bool ModifyUser(ProjectUser user)
        {

            if (!Users.ContainsKey(user.Id))
                return false;
            Users[user.Id] = user;
            return true;
        }

        /// <summary>
        /// Remove member from project.
        /// </summary>
        /// <returns>True if the user was removed, false otherwise.</returns>
        public bool RemoveUser(ProjectUser user)
        {
            return Users.Remove(user.Id);
        }

        public Project(ProjectType type, string name, Guid id, Owner founder)
        {
            Type = type;
            Name = name;
            DateCreated = System.DateTime.Now;
            Id = id;
            Users = new Dictionary<Guid, ProjectUser>();
            Users[founder.Id] = founder;
        }
    }
}
