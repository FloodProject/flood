using System;
using System.Collections.Generic;
using System.Threading.Tasks;
using Flood.Editor.Shared;
using Flood.RPC.Metadata;

namespace Flood.Editor
{
    [Service]
    public interface IProjectManager
    {
        /// <summary>
        /// Adds user to project
        /// </summary>
        Task<bool> AddUserToProject([Id(0)]Guid projectId, [Id(1)]ProjectUser user);

        /// <summary>
        /// Removes user from project
        /// </summary>
        Task<bool> RemoveUserFromProject([Id(0)]Guid projectId, [Id(1)]ProjectUser user);

        /// <summary>
        /// Modifies user permissions 
        /// </summary>
        /// <remarks /> New permissions are passed insed the ProjectUser
        Task<bool> ModifyUserPermissions([Id(0)]Guid projectId, [Id(1)]ProjectUser user);

        /// <summary>
        /// Returns all project users 
        /// </summary>
        Task<ICollection<ProjectUser>> GetUsers([Id(0)]Guid projectId);

        /// <summary>
        /// Returns project 
        /// </summary>
        Task<Project> GetProject([Id(0)]Guid projectId);

        /// <summary>
        /// Read-only dictionary of all the projects.
        /// </summary>
        //Task<Dictionary<Guid, Project>> Projects { get; }

        /// <summary>
        /// Removes an existing project.
        /// </summary>
        /// <remarks>Invokes the ProjectRemoved event.</remarks>
        /// <returns>True if the project was removed, false otherwise.</returns>
        Task<bool> RemoveProject([Id(0)] Guid id);

        /// <summary>
        /// Creates a new project of the given type and name and adds
        /// it to the list.
        /// </summary>
        /// <returns>Returns a new project of this type.</returns>
        Task<Project> CreateProject([Id(0)] ProjectType type, [Id(1)] string name, [Id(2)] Owner founder);

        /// <summary>
        /// Event fired when a project is added.
        /// </summary>
        //event EventHandler<Project> ProjectAdded;

        /// <summary>
        /// Event fired when a project is removed.
        /// </summary>
        //event EventHandler<Project> ProjectRemoved;
    }
}